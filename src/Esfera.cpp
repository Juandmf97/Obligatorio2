#include "../headers/Esfera.h"

#include <cmath>

Esfera::Esfera() : centro(), radio(0.0f) {}

Esfera::Esfera(const Vector3D& c, float r, const Material& mat) : Objeto(mat), centro(c), radio(r) {}

bool Esfera::intersecta(const Rayo& rayo, float& alfa) const {
	float a = rayo.direccion * rayo.direccion;
	float b = (rayo.origen - centro) * 2.0f * rayo.direccion;
	float c = (rayo.origen - centro) * (rayo.origen - centro) - radio * radio;
	float delta = b * b - 4 * a * c;
	alfa = 0.0f;
	if (delta < 0) {
		return false;
	}
	if (delta == 0) {
		alfa = -1.0f * b / (2.0f * a);
		return true;
	}
	alfa = (- 1.0f * b - sqrt(delta)) / (2.0f * a);
	if (alfa > 0) {
		return true;
	}
	alfa = (- 1.0f * b + sqrt(delta)) / (2.0f * a);
	if (alfa > 0) {
		return true;
	}
	return false;
}

Vector3D Esfera::normal(const Vector3D& punto) const {
	return (punto - centro).normalizado();
}
