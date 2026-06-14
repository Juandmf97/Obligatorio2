#pragma once

#include "Color.h"
#include "Vector3D.h"
#include "Rayo.h"
#include "Objeto.h"
#include "Material.h"

class Plano : public Objeto {
public:
	Vector3D punto;
	Vector3D direccionNormal;

	Plano();
	Plano(const Vector3D& p, const Vector3D& n, const Material& mat);
	bool intersecta(const Rayo& rayo, float& alfa) const override;
	Vector3D normal(const Vector3D& punto) const override;
};