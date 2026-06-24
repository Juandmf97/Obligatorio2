#pragma once

#include "Color.h"
#include "Rayo.h"
#include "Vector3D.h"
#include "Material.h"
#include "Interseccion.h"

class Objeto {
public :
	Material material;

	Objeto() {};
	Objeto(const Material& mat) : material(mat) {};
	virtual ~Objeto() {};
	virtual bool intersecta(const Rayo& rayo, Interseccion& inter) = 0;
	virtual Vector3D normal(const Vector3D& punto) const = 0;
};