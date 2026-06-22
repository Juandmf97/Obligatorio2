#pragma once

#include "Objeto.h"

class Triangulo : public Objeto {
private:
	Vector3D v0;
	Vector3D v1;
	Vector3D v2;
	Vector3D normalTriangulo;

public:
	Triangulo();
	Triangulo(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Material& material);

	bool intersecta(const Rayo& rayo, float& alfa) const override;
	Vector3D normal(const Vector3D& punto) const override;
};
