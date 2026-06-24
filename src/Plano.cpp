#include "../headers/Plano.h"

Plano::Plano() {}

Plano::Plano(const Vector3D& p, const Vector3D& n, const Material& mat) : Objeto(mat), punto(p), direccionNormal(n) {}

bool Plano::intersecta(const Rayo& rayo, Interseccion& inter) {
	inter.alfa = 0.0f;
	float divisor = direccionNormal * rayo.direccion;
	if (divisor == 0.0f) {
		return false;
	}
	inter.alfa = ((punto - rayo.origen) * direccionNormal) / divisor;
	if (inter.alfa > 0.0f) {
		inter.objetoIntersectado = this;
		inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
		inter.normal = this->normal(inter.puntoInterseccion);
		return true;
	}
	return false;
}

Vector3D Plano::normal(const Vector3D& punto) const {
	return direccionNormal;
}
