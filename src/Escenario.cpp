#include "../headers/Escenario.h"
#include "../headers/Esfera.h"
#include "../headers/Plano.h"
#include "../headers/Prisma.h"

Escenario::Escenario() {}

Escenario::~Escenario() {
	for (Objeto* objeto : objetos) {
		delete objeto;
	}
	objetos.clear();

	for (Luz* luz : luces) {
		delete luz;
	}
	luces.clear();
}

void Escenario::crearEsfera(const Vector3D& centro, float radio, const Material& material) {
	Objeto* esfera = new Esfera(centro, radio, material);
	objetos.push_back(esfera);
}

void Escenario::crearPlano(const Vector3D& punto, const Vector3D& normal, const Material& material) {
	Objeto* plano = new Plano(punto, normal, material);
	objetos.push_back(plano);
}

void Escenario::crearPrisma(const Vector3D& minimo, const Vector3D& maximo, const Material& material) {
	Objeto* prisma = new Prisma(minimo, maximo);
	objetos.push_back(prisma);
}

void Escenario::agregarLuz(TipoLuz tipo, const Vector3D& origen, const Vector3D& direccion, const Color& color, float intensidad) {
	Luz* luz = new Luz(tipo, origen, direccion, color, intensidad);
	luces.push_back(luz);
}
