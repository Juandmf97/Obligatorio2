#pragma once

#include "Objeto.h"

#include <vector>

struct CaraTriangular {
	int a;
	int b;
	int c;

	CaraTriangular();
	CaraTriangular(int a0, int b0, int c0);
};

class Malla : public Objeto {
private:
	std::vector<Vector3D> vertices;
	std::vector<CaraTriangular> caras;
	mutable Vector3D normalInterseccion;

public:
	Malla();
	Malla(const std::vector<Vector3D>& vertices, const std::vector<CaraTriangular>& caras, const Material& material);

	bool intersecta(const Rayo& rayo, float& alfa) const override;
	Vector3D normal(const Vector3D& punto) const override;
};
