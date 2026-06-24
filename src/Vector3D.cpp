#include "../headers/vector3D.h"

#include <cmath>

Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3D::Vector3D(float x0, float y0, float z0) : x(x0), y(y0), z(z0) {}

Vector3D Vector3D::operator+(const Vector3D& otro) const {
	return Vector3D(x + otro.x, y + otro.y, z + otro.z);
}

Vector3D Vector3D::operator-(const Vector3D& otro) const {
	return Vector3D(x - otro.x, y - otro.y, z - otro.z);
}

float Vector3D::operator*(const Vector3D& otro) const {
	return x * otro.x + y * otro.y + z * otro.z;
}

Vector3D Vector3D::operator*(float escalar) const {
	return Vector3D(x * escalar, y * escalar, z * escalar);
}

Vector3D Vector3D::operator/(float escalar) const {
	if (escalar == 0.0f) {
		return Vector3D();
	}
	return Vector3D(x / escalar, y / escalar, z / escalar);
}

Vector3D Vector3D::cruz(const Vector3D& otro) const {
	return Vector3D(y * otro.z - z * otro.y, z * otro.x - x * otro.z, x * otro.y - y * otro.x);
}

void Vector3D::operator+=(const Vector3D& otro) {
	x += otro.x;
	y += otro.y;
	z += otro.z;
}

void Vector3D::operator-=(const Vector3D& otro) {
	x -= otro.x;
	y -= otro.y;
	z -= otro.z;
}

void Vector3D::operator*=(float escalar) {
	x *= escalar;
	y *= escalar;
	z *= escalar;
}

float Vector3D::modulo() const {
	return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalizado() const {
	float modulo = this->modulo();
	return (*this) / modulo;
}