#include "../headers/Triangulo.h"

#include <cmath>

Triangulo::Triangulo() : v0(), v1(), v2(), normalTriangulo() {}

Triangulo::Triangulo(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Material& material)
	: Objeto(material), v0(a), v1(b), v2(c) {
	normalTriangulo = (v1 - v0).productoVectorial(v2 - v0).normalizado();
}

bool Triangulo::intersecta(const Rayo& rayo, float& alfa) const {
	const float epsilon = 0.000001f;

	Vector3D edge1 = v1 - v0;
	Vector3D edge2 = v2 - v0;
	Vector3D h = rayo.direccion.productoVectorial(edge2);
	float a = edge1 * h;

	if (std::abs(a) < epsilon) {
		return false;
	}

	float f = 1.0f / a;
	Vector3D s = rayo.origen - v0;
	float u = f * (s * h);

	if (u < 0.0f || u > 1.0f) {
		return false;
	}

	Vector3D q = s.productoVectorial(edge1);
	float v = f * (rayo.direccion * q);

	if (v < 0.0f || u + v > 1.0f) {
		return false;
	}

	float t = f * (edge2 * q);
	if (t <= epsilon) {
		return false;
	}

	alfa = t;
	return true;
}

Vector3D Triangulo::normal(const Vector3D& punto) const {
	return normalTriangulo;
}