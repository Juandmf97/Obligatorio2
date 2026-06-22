#include "../headers/Mapeador.h"
#include "../headers/Objeto.h"
#include "../headers/Interseccion.h"
#include <iostream>
#include <cmath>

float Mapeador::atenuar(float d) {
	float c0 = 1;
	float c1 = 0;
	float c2 = 0;
	return std::fmin(1, 1 / (c0 + c1 * d + c2 * d * d));
}

//REVISADA OK
//Mejoras : Que cada objeto cree el inter y no tener que pedirle despues el punto y la normal al Mapeador
bool Mapeador::obtenerInterseccion(const Rayo& rayo, const Escenario& escenario, Interseccion& inter) {
	//Fijo la proyeccion del rayo en el infinito
	inter.alfa = escenario.infinito;
	//Recorro todos los objetos 
	for (Objeto* objeto : escenario.objetos) {
		float alfa;
		//Si uno se intersecta en el camino, me devuelve un alfa numérico que es la distancia al origen del rayo
		//Si ese objeto está por delante del inter.alfa entonces significa que el inter.alfa va a quedar descartado
		//Y este va a tomar su lugar
		if (objeto->intersecta(rayo, alfa) && alfa > 0.0f && alfa < inter.alfa) {
			inter.alfa = alfa;
			inter.objetoIntersectado = objeto;
		}
	}
	//Si no hubo una interseccion descarto
	if (inter.objetoIntersectado == nullptr) {
		return false;
	}
	//Si la hubo coloco el punto de Interseccion calculado a partir del origen y la distancia en la direccion del rayo
	inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
	//Calculo la normal del objeto en ese punto
	inter.normal = inter.objetoIntersectado->normal(inter.puntoInterseccion).normalizado();
	return true;
}

Color Mapeador::calcularLuzTransmitida(const Escenario& escenario, const Interseccion& inter, const Luz* luz) {
	//Obtengo la direccion del vector que parte desde el objeto hacia la luz
	Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
	float d = luz->obtenerDistancia(inter.puntoInterseccion);
	//Creo un rayo que parte desde el punto de Interseccion y va hacia la luz
	Rayo sombra(inter.puntoInterseccion + inter.normal * 0.0001f, L);
	Color luzTransmitida(1, 1, 1);

	//Recorro todos los objetos
	for (Objeto* objeto : escenario.objetos) {
		float alfa;
		//Para todos los objetos que no sean el propio intersectado si el objeto se intersecta con el rayo sombra
		//O sea si se encuentra con un objeto en el camino entre él y la luz 
		if (objeto != inter.objetoIntersectado && objeto->intersecta(sombra, alfa) && alfa > 0.0f && alfa < d) {
			float kRefraccion = objeto->material.kRefraccion;
			if (kRefraccion <= 0.0f) {
				return Color(0, 0, 0);
			}

			luzTransmitida = Color(
				luzTransmitida.r * objeto->material.color.r * kRefraccion,
				luzTransmitida.g * objeto->material.color.g * kRefraccion,
				luzTransmitida.b * objeto->material.color.b * kRefraccion
			);
		}
	}

	return luzTransmitida;
}

Color Mapeador::sombrear(const Luz* luz, const Interseccion& inter, const Color& luzTransmitida, const Rayo& rayoIncidente) {
	//Obtengo la direccion desde el punto de inersección hacia la luz
	Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
	//Calculo vector de Reflexion
	//L = L|| + L-|
	//L = L|| + (L*N)N esto con N normalizado, yo estoy seguro de que está normalizado porque ya lo asegure antes
	//L|| = L - (L*N)*N
	//R = L|| - L-| = L - 2*(L*N)*N
	Vector3D R = L - (inter.normal * 2.0f) * (L * inter.normal);
	R = (R* (-1.0f)).normalizado();
	//Aca rayoIncidente se refiere al rayo que viene del ojo, entonces tomo la direccion desde el objeto al ojo
	Vector3D V = (rayoIncidente.direccion * -1).normalizado();
	float distancia = luz->obtenerDistancia(inter.puntoInterseccion);
	//Calculo la distancia de la luz al punto de Interseccion y realizo la atenuacion por la distancia a la misma
	float factorAtenuacion = atenuar(distancia);
	float factorDifuso = std::fmax(0.0f, inter.normal * L) * inter.objetoIntersectado->material.kDifusa;
	float factorPhong = pow(std::fmax(0.0f, R * V), inter.objetoIntersectado->material.nPhong) * inter.objetoIntersectado->material.kEspecular;
	Color colorLuz = luz->color * luz->intensidad;

	Color luzDifusa(
		inter.objetoIntersectado->material.color.r * colorLuz.r * factorDifuso,
		inter.objetoIntersectado->material.color.g * colorLuz.g * factorDifuso,
		inter.objetoIntersectado->material.color.b * colorLuz.b * factorDifuso
	);

	Color luzPhong = colorLuz * factorPhong;
	Color luzTotal = (luzDifusa + luzPhong) * factorAtenuacion;

	return Color(
		luzTotal.r * luzTransmitida.r,
		luzTotal.g * luzTransmitida.g,
		luzTotal.b * luzTransmitida.b
	);
}

//REVISADA OK
//Mejoras : Se podría poner un Color de luz Ambiente a futuro
Color Mapeador::calcularIluminacion(const Color& luzDifusa, const Interseccion& inter, const Escenario& escenario) {
	//Obtengo la intensidad de la luz ambiente de la escena
	float intensidadAmbiente = escenario.luzAmbiente;
	//Calculo el valor de color de la luz ambiente como el color del material * su kAmbiente * la intensidad de la luz 
	//Aca no uso Color de luz ambiente porque es nomas para que las sombras y eso no sean duras negras
	Color luzAmbiente = inter.objetoIntersectado->material.color * inter.objetoIntersectado->material.kAmbiente * intensidadAmbiente;
	//Lo sumo con las componentes de Difusa y Phong que había calculado antes
	Color luzTotal = luzAmbiente + luzDifusa;
	return luzTotal;
}

//REVISADA OK
//Mejoras : no por ahora
Color Mapeador::calcularReflexion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad, const bool TIR) {
	float kRfx;
	if (TIR) {
		kRfx = 1.0f;
	}
	else {
		kRfx = inter.objetoIntersectado->material.kReflexion;
	}
	//Si el objeto no es reflectante devuelvo Color(0,0,0) para que no aporte nada en la suma posterior
	if (kRfx <= 0.0f) {
		return Color(0, 0, 0);
	}
	//Tomo I como la direccion desde el ojo hacia el punto de Interseccion
	Vector3D I = rayoIncidente.direccion.normalizado();
	//Calculo la reflexion DESDE EL OJO, no desde la luz porque estoy buscando qué veo yo cuando mi vista refleja en el objeto
	Vector3D R = I - (inter.normal * 2.0f) * (I * inter.normal);
	R = R.normalizado();
	//Salgo desde un poquito más adelante del punto de Intersección y en la dirección del rayo reflejado a ver
	//con qué me encuentro. Agrego ese 0.001f para que no salten problemas de autovisión
	Rayo reflejado(inter.puntoInterseccion + R * 0.001f, R);
	//Aca busco a ver si me encuentro con algo en el camino, o sea hago todo el algoritmo de interseccion pero
	//con la camara colocada en el punto de Interseccion y mirando en la direccion de la reflexion
	Color colorReflejado = this->interseccion(reflejado, escenario, profundidad + 1);
	return colorReflejado;
}

//Revisada OK
//Mejoras : No por ahora
Color Mapeador::calcularRefraccion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad) {
	float kRfr = inter.objetoIntersectado->material.kRefraccion;
	float indiceRefraccion = inter.objetoIntersectado->material.nRefraccion;
	//Si no hay refraccion esta componente no contribuye
	if (kRfr <= 0.0f || indiceRefraccion <= 0.0f) {
		return Color(0, 0, 0);
	}
	//Miro desde la camara hacia el objeto
	Vector3D I = rayoIncidente.direccion.normalizado();
	Vector3D N = inter.normal;
	//Aca estoy tomando como medio incial el aire (n1 = 1.0f), el medio 2 es el objeto
	float n1 = 1.0f;
	float n2 = indiceRefraccion;
	//Calculo el coseno y si este es mayor a 0 voy a invertir los medios porque significa que estoy saliendo del objeto
	float coseno = I * N;
	if (coseno > 0.0f) {
		float n0 = n1;
		n1 = n2;
		n2 = n0;
		N = N * (-1.0f);
	}
	//si es menor a cero o cero uso el opuesto (para que sea siempre positivo)
	else {
		coseno = -1 * coseno;
	}
	//Calculo el factor entre los coeficientes de Snell y aplico la ley de Snell para determinar el coseno saliente
	float factor = n1 / n2;
	float nuevoCosenoCuadrado = 1 - factor * factor * (1 - coseno * coseno);
	//Si el nuevo coseno es menor a 0 calculo reflexion (significa que vuelve al medio inicial) entonces hago una 
	//reflexion pero sujeta a TIR
	if (nuevoCosenoCuadrado < 0) {
		return calcularReflexion(rayoIncidente, inter, escenario, profundidad, true);
	}
	//Sino calculo la direccion de refraccion y hago una interseccion desde ahi
	float nuevoCoseno = sqrt(nuevoCosenoCuadrado);
	Vector3D direccionRefraccion = (I * factor + N * (factor * coseno - nuevoCoseno)).normalizado();
	Rayo rayo(inter.puntoInterseccion - N * 0.001f, direccionRefraccion);
	Color colorRefraccion = interseccion(rayo, escenario, profundidad + 1);
	return colorRefraccion;
}

Color Mapeador::calcularColor(const Interseccion& inter, const Color& colorReflejado, const Color& colorDifuso, const Color& colorRefractado) {
	float kRfx = inter.objetoIntersectado->material.kReflexion;
	float kRfr = inter.objetoIntersectado->material.kRefraccion;
	return colorDifuso * (1 - kRfx - kRfr) + colorReflejado * kRfx + colorRefractado * kRfr;
}

Color Mapeador::interseccion(const Rayo& rayo, const Escenario& escenario, int profundidad) {
	
	//BUSCO EL OBJETO INTERSECTADO MÁS CERCANO Y PEGO LOS DATOS EN EL INTER
	Interseccion inter;
	bool intersecta = obtenerInterseccion(rayo, escenario, inter);

	if (intersecta) {
		Color luzDifusa = Color(0,0,0);
		
		//PARA CADA UNA DE LAS LUCES VOY A CALCULAR LA SOMBRA QUE SE PROYECTA
		for (Luz* luz : escenario.luces) {
			//BUSCO SI PROYECTANDO DESDE EL OBJETO HACIA LA LUZ ME CRUZO CON ALGUNA COSA
			Color luzTransmitida = calcularLuzTransmitida(escenario, inter, luz);
			//AGREGO LA ATENUACION QUE CORRESPONDA
			luzDifusa = luzDifusa + sombrear(luz, inter, luzTransmitida, rayo);
		}
		
		//CALCULO LA INTENSIDAD DADA POR TODAS LAS LUCES DIFUSAS Y AMBIENTE
		Color colorDifuso = calcularIluminacion(luzDifusa, inter, escenario);

		if (profundidad >= 3) {
			return colorDifuso;
		}

		Color colorReflejado = calcularReflexion(rayo, inter, escenario, profundidad, false);
		Color colorRefractado = calcularRefraccion(rayo, inter, escenario, profundidad);
		Color colorFinal = calcularColor(inter, colorReflejado, colorDifuso, colorRefractado);
		return colorFinal;
	}

	return Color(0.0f, 0.0f, 1.0f);
}

