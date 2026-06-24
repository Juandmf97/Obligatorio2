#include "../headers/Prisma.h"
#include <cmath>

Prisma::Prisma(Vector3D min, Vector3D max, const Material& mat) : Objeto(mat) {
	vertices.push_back(Vector3D(min.x, min.y, min.z)); //TRASERO-IZQUIERDA-ABAJO
	vertices.push_back(Vector3D(max.x, min.y, min.z)); //TRASERO-DERECHA-ABAJO
	vertices.push_back(Vector3D(max.x, max.y, min.z)); //TRASERO-DERECHA-ARRIBA
	vertices.push_back(Vector3D(min.x, max.y, min.z)); //TRASERO-IZQUIERDA-ARRIBA
	vertices.push_back(Vector3D(min.x, min.y, max.z)); //FRENTE-IZQUIERDA-ABAJO
	vertices.push_back(Vector3D(max.x, min.y, max.z)); //FRENTE-DERECHA-ABAJO
	vertices.push_back(Vector3D(max.x, max.y, max.z)); //FRENTE-DERECHA-ARRIBA
	vertices.push_back(Vector3D(min.x, max.y, max.z)); //FRENTE-IZQUIERDA-ARRIBA
	//CARA TRASERA
	triangulos.push_back(Triangulo(1, 0, 3));
	triangulos.push_back(Triangulo(1, 3, 2));
	//CARA FRONTAL
	triangulos.push_back(Triangulo(4, 5, 6));
	triangulos.push_back(Triangulo(4, 6, 7));
	//CARA INFERIOR 
	triangulos.push_back(Triangulo(0, 1, 5));
	triangulos.push_back(Triangulo(0, 5, 4));
	//CARA SUPERIOR
	triangulos.push_back(Triangulo(7, 6, 2));
	triangulos.push_back(Triangulo(7, 2, 3));
	//CARA IZQUIERDA
	triangulos.push_back(Triangulo(0, 4, 7));
	triangulos.push_back(Triangulo(0, 7, 3));
	//CARA DERECHA
	triangulos.push_back(Triangulo(5, 1, 2));
	triangulos.push_back(Triangulo(5, 2, 6));
}

bool Prisma::intersecta(const Rayo& rayo, Interseccion& inter) {
	inter.alfa = 1e10f;
	bool hit = false;
	for (const auto& t : triangulos) {
		const Vector3D& w1 = vertices[t.v1];
		const Vector3D& w2 = vertices[t.v2];
		const Vector3D& w3 = vertices[t.v3];
		Interseccion temp;
		if (t.interseccion(rayo, w1, w2, w3, temp) && temp.alfa > 0.0f && temp.alfa < inter.alfa) {
			inter.alfa = temp.alfa;
			inter.puntoInterseccion = temp.puntoInterseccion;
			inter.normal = temp.normal;
			inter.objetoIntersectado = this;
			hit = true;
		}
	}
	
	return hit;
}

Vector3D Prisma::normal(const Vector3D& punto) const {
	return Vector3D(0, 0, 0);
}