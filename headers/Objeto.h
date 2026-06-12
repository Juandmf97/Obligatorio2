#pragma once

#include "Color.h"
#include "Rayo.h"
#include "Vector3D.h"

class Objeto {
public :
	Color color;

	Objeto() {};
	Objeto(const Color& col) :color(col) {};
	virtual ~Objeto() {};
	virtual bool intersecta(const Rayo& rayo, float& alfa) const = 0;
	virtual Vector3D normal(const Vector3D& punto) const = 0;
};