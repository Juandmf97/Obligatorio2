#pragma once

#include "Vector3D.h"
#include "Color.h"

class Luz {
public :
	Vector3D direccion;
	Color color;

	Luz(const Vector3D& d, const Color& c);
};