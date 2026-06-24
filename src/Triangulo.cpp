#include "../headers/Triangulo.h"
#include <cmath>

Triangulo::Triangulo(int vertice1, int vertice2, int vertice3) : v1(vertice1), v2(vertice2), v3(vertice3) {}

bool Triangulo::interseccion(const Rayo& rayo, const Vector3D& w1, const Vector3D& w2, const Vector3D& w3, Interseccion& inter) const {
	float epsilon = 1e-6f;
	Vector3D arista1 = w2 - w1;
	Vector3D arista2 = w3 - w1;

	Vector3D rayoCruzArista2 = rayo.direccion.cruz(arista2);
	float det = arista1 * rayoCruzArista2;
	
	if (fabs(det) < epsilon) {
		return false;
	}

	float inv_det = 1.0f / det;
	Vector3D s = rayo.origen - w1;
	float u = inv_det * (s * rayoCruzArista2);

	if (u < -epsilon || u - 1 > epsilon) {
		return false;
	}

	Vector3D sCruzArista1 = s.cruz(arista1);
	float v = inv_det * (rayo.direccion * sCruzArista1);

	if (v < -epsilon || u + v - 1 > epsilon) {
		return false;
	}

	float t = inv_det * (arista2 * sCruzArista1);

	if (t > epsilon) {
		inter.alfa = t;
		Vector3D n = arista1.cruz(arista2).normalizado();

		// FORZAR ORIENTACIÓN HACIA EL RAYO
		if (n * rayo.direccion > 0)
			n = n * -1;

		inter.normal = n;
		inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
		return true;
	}

	return false;
}