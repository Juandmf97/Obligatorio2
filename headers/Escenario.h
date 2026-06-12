#pragma once

#include "Objeto.h"
#include "Luz.h"

#include <vector>

class Escenario {
public :
	float zMax = 1e10f;
	std::vector<Objeto*> objetos;
	std::vector<Luz*> luces;
	float luzAmbiente = 0.1f;

	Escenario();
	~Escenario();
	void crearEsfera(const Vector3D& centro, float radio, const Color& color);
	void crearPlano(const Vector3D& punto, const Vector3D& normal, const Color& color);
	void agregarLuz(const Vector3D& direccion, const Color& color);
};