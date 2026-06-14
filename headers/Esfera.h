#pragma once
#include "Objeto.h"
#include "Vector3D.h"
#include "Rayo.h"
#include "Color.h"

class Esfera : public Objeto {
public :
	Vector3D centro;
	float radio;

	Esfera();
	Esfera(const Vector3D& c, float r, const Material& mat);
	bool intersecta(const Rayo& rayo, float& alfa) const override;
	Vector3D normal(const Vector3D& punto) const override;
};