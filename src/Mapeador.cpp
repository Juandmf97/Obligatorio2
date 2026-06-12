#include "../headers/Mapeador.h"
#include "../headers/Objeto.h"
#include <iostream>

Color Mapeador::interseccion(const Rayo& rayo, const Escenario& escenario) {
	bool intersecta = false;
	float alfaMin = escenario.zMax;
	Objeto* objetoIntersectado = nullptr;

	for (Objeto* objeto : escenario.objetos) {
		float alfa;
		if (objeto->intersecta(rayo , alfa) && alfa > 0.0f && alfa < alfaMin) {
			alfaMin = alfa;
			objetoIntersectado = objeto;
			intersecta = true;
		}
	}

	if (intersecta) {
		Vector3D L = escenario.luces[0]->direccion.normalizado() * -1;
		Vector3D puntoCorte = rayo.origen + rayo.direccion * alfaMin;
		Vector3D N = objetoIntersectado->normal(puntoCorte);
		Rayo sombra(puntoCorte + N * 0.0001f, L);
		bool sombreado = false;

		for (Objeto* objeto : escenario.objetos) {
			float alfa;
			if (objeto != objetoIntersectado && objeto->intersecta(sombra, alfa) && alfa > 0.0f) {
				sombreado = true;
				break;
			}
		}

		float luzDifusa = 0.0f;
		float luzAmbiente = escenario.luzAmbiente;

		if (!sombreado) {
			luzDifusa = N * L;
			if (luzDifusa < 0.0f) {
				luzDifusa = 0.0f;
			}
		}
		float intensidad = luzAmbiente + luzDifusa;
		if (intensidad > 1.0f) {
			intensidad = 1.0f;
		}
		return objetoIntersectado->color * (intensidad);
	}

	return Color(0.0f, 0.0f, 1.0f);
}

