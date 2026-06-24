#pragma once

#include <vector>
#include "Objeto.h"
#include "Triangulo.h"

class Prisma : public Objeto {
public:
	std::vector<Vector3D> vertices;
	std::vector<Triangulo> triangulos;

	Prisma(Vector3D min, Vector3D max, const Material& mat);
	bool intersecta(const Rayo& rayo, Interseccion& inter) override;
	Vector3D normal(const Vector3D& punto) const;
};