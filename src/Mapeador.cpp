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

bool Mapeador::obtenerInterseccion(const Rayo& rayo, const Escenario& escenario, Interseccion& inter) {
	inter.alfa = escenario.infinito;

	for (Objeto* objeto : escenario.objetos) {
		float alfa;
		if (objeto->intersecta(rayo, alfa) && alfa > 0.0f && alfa < inter.alfa) {
			inter.alfa = alfa;
			inter.objetoIntersectado = objeto;
		}
	}
	if (inter.objetoIntersectado == nullptr) {
		return false;
	}

	inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
	inter.normal = inter.objetoIntersectado->normal(inter.puntoInterseccion);
	return true;
}

Color Mapeador::calcularLuzTransmitida(const Escenario& escenario, const Interseccion& inter, const Luz* luz) {
	Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
	float d = luz->obtenerDistancia(inter.puntoInterseccion);
	Rayo sombra(inter.puntoInterseccion + inter.normal * 0.0001f, L);
	Color luzTransmitida(1, 1, 1);

	for (Objeto* objeto : escenario.objetos) {
		float alfa;
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
	Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
	Vector3D R = L - (inter.normal * 2.0f) * (L * inter.normal);
	R = (R* (-1.0f)).normalizado();
	Vector3D V = (rayoIncidente.direccion * -1).normalizado();
	float distancia = luz->obtenerDistancia(inter.puntoInterseccion);
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
Color Mapeador::calcularIluminacion(const Color& luzDifusa, const Interseccion& inter, const Escenario& escenario) {
	float intensidadAmbiente = escenario.luzAmbiente;
	Color luzAmbiente = inter.objetoIntersectado->material.color * inter.objetoIntersectado->material.kAmbiente * intensidadAmbiente;
	Color luzTotal = luzAmbiente + luzDifusa;
	return luzTotal;
}

Color Mapeador::calcularReflexion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad) {
	float kRfx = inter.objetoIntersectado->material.kReflexion;

	if (kRfx <= 0.0f) {
		return Color(0, 0, 0);
	}

	Vector3D I = rayoIncidente.direccion.normalizado();
	Vector3D R = I - (inter.normal * 2.0f) * (I * inter.normal);
	R = R.normalizado();

	Rayo reflejado(inter.puntoInterseccion + inter.normal * 0.001f, R);
	Color colorReflejado = this->interseccion(reflejado, escenario, profundidad + 1);
	return colorReflejado;
}

Color Mapeador::calcularRefraccion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad) {
	float kRfr = inter.objetoIntersectado->material.kRefraccion;
	float indiceRefraccion = inter.objetoIntersectado->material.nRefraccion;

	if (kRfr <= 0.0f || indiceRefraccion <= 0.0f) {
		return Color(0, 0, 0);
	}

	Vector3D I = rayoIncidente.direccion.normalizado();
	Vector3D N = inter.normal;

	float n1 = 1.0f;
	float n2 = indiceRefraccion;
	float coseno = I * N;
	if (coseno > 0.0f) {
		float n0 = n1;
		n1 = n2;
		n2 = n0;
		N = N * (-1.0f);
	}
	else {
		coseno = -1 * coseno;
	}
	float factor = n1 / n2;
	float nuevoCosenoCuadrado = 1 - factor * factor * (1 - coseno * coseno);
	if (nuevoCosenoCuadrado < 0) {
		return calcularReflexion(rayoIncidente, inter, escenario, profundidad);
	}
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

		Color colorReflejado = calcularReflexion(rayo, inter, escenario, profundidad);
		Color colorRefractado = calcularRefraccion(rayo, inter, escenario, profundidad);
		Color colorFinal = calcularColor(inter, colorReflejado, colorDifuso, colorRefractado);
		return colorFinal;
	}

	return Color(0.0f, 0.0f, 1.0f);
}

