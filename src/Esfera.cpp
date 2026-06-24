#include "../headers/Esfera.h"

#include <cmath>

Esfera::Esfera() : centro(), radio(0.0f) {}

Esfera::Esfera(const Vector3D& c, float r, const Material& mat) : Objeto(mat), centro(c), radio(r) {}

bool Esfera::intersecta(const Rayo& rayo, Interseccion& inter) {
	bool hit = false;
	float a = rayo.direccion * rayo.direccion;
	float b = (rayo.origen - centro) * 2.0f * rayo.direccion;
	float c = (rayo.origen - centro) * (rayo.origen - centro) - radio * radio;
	float delta = b * b - 4 * a * c;
	inter.alfa = 0.0f;
	if (delta < 0) {
		return false;
	}
	if (delta == 0) {
		inter.alfa = -1.0f * b / (2.0f * a);
		hit = true;
	}
	else {
		inter.alfa = (-1.0f * b - sqrt(delta)) / (2.0f * a);
		if (inter.alfa > 0) {
			hit = true;
		}
		else {
			inter.alfa = (-1.0f * b + sqrt(delta)) / (2.0f * a);
			if (inter.alfa > 0) {
				hit = true;
			}
		}
	}
	if (hit) {
		inter.objetoIntersectado = this;
		inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
		inter.normal = this->normal(inter.puntoInterseccion);
	}
	
	return hit;
}

Vector3D Esfera::normal(const Vector3D& punto) const {
	return (punto - centro).normalizado();
}
