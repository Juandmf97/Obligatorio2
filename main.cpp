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

	escena.crearPrisma(Vector3D(-1, -1, -2), Vector3D(0, -0.5, -1),Material(Color(1,1,1),1,1,0,0,0,1,0));

	Renderizador renderizador;
	renderizador.render(escena, config);
	return 0;
}
