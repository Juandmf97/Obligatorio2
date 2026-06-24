#pragma once

#include "Vector3D.h"

class Objeto;

class Interseccion {
public :
	Objeto* objetoIntersectado = nullptr;
	float alfa = 0.0f;
	Vector3D puntoInterseccion;
	Vector3D normal;
};