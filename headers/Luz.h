#pragma once

#include "Vector3D.h"
#include "Color.h"

enum TipoLuz {
	PUNTUAL,
	DIRECCIONAL
};

class Luz {
public :
	TipoLuz tipo;
	Vector3D origen;
	Vector3D direccion;
	Color color;
	float intensidad;

	Luz(TipoLuz t, const Vector3D& o, const Vector3D& d, const Color& c, float i);
	Vector3D obtenerDireccion(const Vector3D& punto) const;
	float obtenerDistancia(const Vector3D& punto) const;
};