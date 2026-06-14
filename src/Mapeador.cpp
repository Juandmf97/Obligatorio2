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

bool Mapeador::estaSombreado(const Escenario& escenario, const Interseccion& inter, const Luz* luz) {
	Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
	float d = luz->obtenerDistancia(inter.puntoInterseccion);
	Rayo sombra(inter.puntoInterseccion + inter.normal * 0.0001f, L);
	bool sombreado = false;

	for (Objeto* objeto : escenario.objetos) {
		float alfa;
		if (objeto != inter.objetoIntersectado && objeto->intersecta(sombra, alfa) && alfa > 0.0f && alfa < d) {
			sombreado = true;
			break;
		}
	}

	return sombreado;
}

float Mapeador::sombrear(const Luz* luz, const Interseccion& inter, const bool sombreado) {
	float luzDifusa = 0.0f;
	if (!sombreado) {
		Vector3D L = luz->obtenerDireccion(inter.puntoInterseccion) * -1;
		float distancia = luz->obtenerDistancia(inter.puntoInterseccion);
		float factorAtenuacion = atenuar(distancia);
		luzDifusa = inter.normal * L * factorAtenuacion;
		if (luzDifusa < 0.0f) {
			luzDifusa = 0.0f;
		}
	}
	return luzDifusa;
}

Color Mapeador::calcularIluminacion(float luzDifusa, const Interseccion& inter, const Escenario& escenario) {
	float intensidadAmbiente = escenario.luzAmbiente;
	float luzAmbiente = inter.objetoIntersectado->material.kAmbiente * intensidadAmbiente;
	luzDifusa *= inter.objetoIntersectado->material.kDifusa;
	float intensidad = luzAmbiente + luzDifusa;
	if (intensidad > 1.0f) {
		intensidad = 1.0f;
	}

	Color colorDifuso = inter.objetoIntersectado->material.color * intensidad;
	return colorDifuso;
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

Color Mapeador::calcularColor(const Interseccion& inter, const Color& colorReflejado, const Color& colorDifuso) {
	float kRfx = inter.objetoIntersectado->material.kReflexion;
	return colorDifuso * (1 - kRfx) + colorReflejado * kRfx;
}

Color Mapeador::interseccion(const Rayo& rayo, const Escenario& escenario, int profundidad) {
	
	//BUSCO EL OBJETO INTERSECTADO MÁS CERCANO Y PEGO LOS DATOS EN EL INTER
	Interseccion inter;
	bool intersecta = obtenerInterseccion(rayo, escenario, inter);

	if (intersecta) {
		float luzDifusa = 0.0f;
		
		//PARA CADA UNA DE LAS LUCES VOY A CALCULAR LA SOMBRA QUE SE PROYECTA
		for (Luz* luz : escenario.luces) {
			//BUSCO SI PROYECTANDO DESDE EL OBJETO HACIA LA LUZ ME CRUZO CON ALGUNA COSA
			bool sombreado = estaSombreado(escenario, inter, luz);
			//AGREGO LA ATENUACION QUE CORRESPONDA
			luzDifusa += sombrear(luz, inter, sombreado);
		}
		
		//CALCULO LA INTENSIDAD DADA POR TODAS LAS LUCES DIFUSAS Y AMBIENTE
		Color colorDifuso = calcularIluminacion(luzDifusa, inter, escenario);

		if (profundidad >= 3) {
			return colorDifuso;
		}

		Color colorReflejado = calcularReflexion(rayo, inter, escenario, profundidad);
		Color colorFinal = calcularColor(inter, colorReflejado, colorDifuso);
		return colorFinal;
	}

	return Color(0.0f, 0.0f, 1.0f);
}

