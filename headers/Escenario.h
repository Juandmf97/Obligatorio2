#pragma once

#include "Objeto.h"
#include "Luz.h"

#include <vector>

class Escenario {
public :
	const float infinito = 1e10f;
	std::vector<Objeto*> objetos;
	std::vector<Luz*> luces;
	float luzAmbiente = 0.1f;

	Escenario();
	~Escenario();
	void crearEsfera(const Vector3D& centro, float radio, const Material& material);
	void crearPlano(const Vector3D& punto, const Vector3D& normal, const Material& material);
	void crearPrisma(const Vector3D& minimo, const Vector3D& maximo, const Material& material);
	void agregarLuz(TipoLuz tipo, const Vector3D& origen, const Vector3D& direccion, const Color& color, float intensidad);
	void crearCilindro(const Vector3D& centro, float radio, float altura, const Material& material);
};