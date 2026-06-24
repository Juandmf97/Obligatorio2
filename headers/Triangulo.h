#pragma once

#include "Rayo.h"
#include "Interseccion.h"

class Triangulo {
public:
	int v1, v2, v3;

	Triangulo(int vertice1, int vertice2, int vertice3);
	bool interseccion(const Rayo& rayo, const Vector3D& w1, const Vector3D& w2, const Vector3D& w3, Interseccion& inter) const;
};