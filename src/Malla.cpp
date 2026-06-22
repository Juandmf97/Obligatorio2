#include "../headers/Malla.h"
#include "../headers/Triangulo.h"

CaraTriangular::CaraTriangular() : a(0), b(0), c(0) {}

CaraTriangular::CaraTriangular(int a0, int b0, int c0) : a(a0), b(b0), c(c0) {}

Malla::Malla() : vertices(), caras(), normalInterseccion() {}

Malla::Malla(const std::vector<Vector3D>& vertices0, const std::vector<CaraTriangular>& caras0, const Material& material)
	: Objeto(material), vertices(vertices0), caras(caras0), normalInterseccion() {}

bool Malla::intersecta(const Rayo& rayo, float& alfa) const {
	bool hayInterseccion = false;
	float alfaMinimo = 1e20f;
	Vector3D normalMasCercana;

	for (const CaraTriangular& cara : caras) {
		if (cara.a < 0 || cara.b < 0 || cara.c < 0 ||
			cara.a >= (int)vertices.size() || cara.b >= (int)vertices.size() || cara.c >= (int)vertices.size()) {
			continue;
		}

		Triangulo triangulo(vertices[cara.a], vertices[cara.b], vertices[cara.c], material);
		float alfaTriangulo = 0.0f;
		if (triangulo.intersecta(rayo, alfaTriangulo) && alfaTriangulo < alfaMinimo) {
			alfaMinimo = alfaTriangulo;
			normalMasCercana = triangulo.normal(Vector3D());
			hayInterseccion = true;
		}
	}

	if (!hayInterseccion) {
		return false;
	}

	alfa = alfaMinimo;
	normalInterseccion = normalMasCercana;
	return true;
}

Vector3D Malla::normal(const Vector3D& punto) const {
	return normalInterseccion;
}
