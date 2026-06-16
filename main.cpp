#include <iostream>
#include <fstream>

#include "headers/CargadorXML.h"
#include "headers/Escenario.h"
#include "headers/Color.h"
#include "headers/Mapeador.h"

using namespace std;

void render(const Escenario& escenario, const ConfigRender& config)
{
	int ancho = config.ancho;
	int alto = config.alto;
	float ratio = (float)ancho / (float)alto;

	std::ofstream file(config.salida);

	file << "P3\n" << ancho << " " << alto << "\n255\n";

	Vector3D origen = config.camaraOrigen;

	for (int y = alto - 1; y >= 0; y--)
	{
		for (int x = 0; x < ancho; x++)
		{
			float u = ((2.0f * x / ancho) - 1.0f) * ratio;
			float v = (2.0f * y / alto) - 1.0f;

			Vector3D dir(u, v, -config.distanciaPlano);
			dir = dir.normalizado();

			Rayo rayo(origen, dir);

			Mapeador mapeador;

			Color c = mapeador.interseccion(rayo, escenario, 0);

			file << (int)(c.r * 255) << " "
				<< (int)(c.g * 255) << " "
				<< (int)(c.b * 255) << "\n";
		}
	}

	file.close();
}

int main(int argc, char *argv[]) {
	Escenario escena;
	ConfigRender config;
	std::string rutaEscena = argc > 1 ? argv[1] : "escena.xml";

	if (!CargadorXML::cargar(rutaEscena, escena, config)) {
		return 1;
	}

	render(escena, config);
	return 0;
}
