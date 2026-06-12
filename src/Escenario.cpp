#include "../headers/Escenario.h"
#include "../headers/Esfera.h"
#include "../headers/Plano.h"

Escenario::Escenario() {}

Escenario::~Escenario() {
	for (Objeto* objeto : objetos) {
		delete objeto;
	}
	objetos.clear();
}

void Escenario::crearEsfera(const Vector3D& centro, float radio, const Color& color) {
	Objeto* esfera = new Esfera(centro, radio, color);
	objetos.push_back(esfera);
}

void Escenario::crearPlano(const Vector3D& punto, const Vector3D& normal, const Color& color) {
	Objeto* plano = new Plano(punto, normal, color);
	objetos.push_back(plano);
}

void Escenario::agregarLuz(const Vector3D& direccion, const Color& color) {
	Luz* luz = new Luz(direccion, color);
	luces.push_back(luz);
}