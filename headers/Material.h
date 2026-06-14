#pragma once

#include "Color.h"

class Material {
public :
	Color color;
	float kAmbiente;
	float kDifusa;
	float kReflexion;
	float kRefraccion;
	float kEspecular;
	float nPhong;
	float nRefraccion;

	Material();
	Material(const Color& col, float kAmb, float kDif, float kRfx, float kRfr, float kEsp, float nPh, float nRf);
};