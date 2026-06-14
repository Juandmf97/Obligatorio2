#pragma once

#include "Vector3D.h"
#include "Objeto.h"

class Interseccion {
public :
	Objeto* objetoIntersectado = nullptr;
	float alfa = 0.0f;
	Vector3D puntoInterseccion;
	Vector3D normal;
};