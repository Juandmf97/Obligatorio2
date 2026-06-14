#include "../headers/Material.h"

Material::Material() {}

Material::Material(const Color& col, float kAmb, float kDif, float kRfx, float kRfr) : 
	color(col), kAmbiente(kAmb), kDifusa(kDif), kReflexion(kRfx), kRefraccion(kRfr) {}