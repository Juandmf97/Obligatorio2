#include "../headers/Plano.h"

Plano::Plano() {}

Plano::Plano(const Vector3D& p, const Vector3D& n, const Material& mat) : Objeto(mat), punto(p), direccionNormal(n) {}

bool Plano::intersecta(const Rayo& rayo, float& alfa) const {
	alfa = 0.0f;
	float divisor = direccionNormal * rayo.direccion;
	if (divisor == 0.0f) {
		return false;
	}
	alfa = ((punto - rayo.origen) * direccionNormal) / divisor;
	if (alfa > 0.0f) {
		return true;
	}
	return false;
}

Vector3D Plano::normal(const Vector3D& punto) const {
	return direccionNormal;
}
