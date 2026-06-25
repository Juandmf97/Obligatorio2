#pragma once

#include "Escenario.h"
#include "Vector3D.h"

#include <string>

struct ConfigRender {
	int ancho =800;
	int alto = 600;
	std::string salida = "imagen.ppm";
	Vector3D camaraOrigen = Vector3D(0, 4.5, 1);
	Vector3D camaraObjetivo = Vector3D(0, 4.5, -10);
	Vector3D camaraArriba = Vector3D(0, 1, 0);
	float distanciaPlano = 1.0f;
	int factorAA = 1;
};

class CargadorXML {
public:
	static bool cargar(const std::string& ruta, Escenario& escenario, ConfigRender& config);
};
