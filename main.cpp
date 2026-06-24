#include <iostream>
#include <fstream>
#include <string>

#include "headers/CargadorXML.h"
#include "headers/Escenario.h"
#include "headers/Renderizador.h"

using namespace std;

int main(int argc, char *argv[]) {
	Escenario escena;
	ConfigRender config;
	std::string rutaEscena = argc > 1 ? argv[1] : "escena.xml";

	if (argc == 1 && !std::ifstream(rutaEscena).good()) {
		rutaEscena = "../escena.xml";
	}

	if (!CargadorXML::cargar(rutaEscena, escena, config)) {
		return 1;
	}

	//escena.crearPrisma(Vector3D(-4, -1, -8), Vector3D(4, 1, -4), Material(Color(0.8, 0.4, 0.2), 1, 1, 0, 0, 0, 1, 0));
	
	/*
	<prisma punto1="-4 -1 -8" punto2="4 3 -4" material="plasticoBlanco" />
	*/


	Renderizador renderizador;
	renderizador.render(escena, config);
	return 0;
}
