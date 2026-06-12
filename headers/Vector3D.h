#pragma once

class Vector3D {
public :
	float x, y, z;

	Vector3D();
	Vector3D(float x0, float y0, float z0);

	Vector3D operator+(const Vector3D& otro) const;
	Vector3D operator-(const Vector3D& otro) const;
	float operator*(const Vector3D& otro) const;
	Vector3D operator*(float escalar) const;
	Vector3D operator/(float escalar) const;

	void operator+=(const Vector3D& otro);
	void operator-=(const Vector3D& otro);
	void operator*=(float escalar);

	float modulo() const;
	Vector3D normalizado() const;
};