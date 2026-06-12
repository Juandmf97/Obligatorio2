#pragma once
#include "vector3D.h"

class Rayo {
public :
	Vector3D origen, direccion;

	Rayo();
	Rayo(const Vector3D& o, const Vector3D& d);
};