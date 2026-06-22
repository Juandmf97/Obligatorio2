#include "../headers/Renderizador.h"

#include "../headers/Color.h"
#include "../headers/Interseccion.h"
#include "../headers/Mapeador.h"
#include "../headers/Rayo.h"

#include <fstream>
#include <string>

namespace {
	std::string agregarSufijo(const std::string& ruta, const std::string& sufijo)
	{
		size_t punto = ruta.find_last_of('.');
		if (punto == std::string::npos) {
			return ruta + sufijo;
		}
		return ruta.substr(0, punto) + sufijo + ruta.substr(punto);
	}

	void escribirColor(std::ofstream& file, const Color& c)
	{
		file << (int)(c.r * 255) << " "
			<< (int)(c.g * 255) << " "
			<< (int)(c.b * 255) << "\n";
	}

	void escribirGris(std::ofstream& file, float valor)
	{
		if (valor < 0.0f) {
			valor = 0.0f;
		}
		if (valor > 1.0f) {
			valor = 1.0f;
		}

		int gris = (int)(valor * 255);
		file << gris << " " << gris << " " << gris << "\n";
	}
}

void Renderizador::render(const Escenario& escenario, const ConfigRender& config)
{
	int ancho = config.ancho;
	int alto = config.alto;
	float ratio = (float)ancho / (float)alto;

	std::ofstream file(config.salida);
	std::ofstream fileReflexion(agregarSufijo(config.salida, "_reflexion"));
	std::ofstream fileTransmision(agregarSufijo(config.salida, "_transmision"));

	file << "P3\n" << ancho << " " << alto << "\n255\n";
	fileReflexion << "P3\n" << ancho << " " << alto << "\n255\n";
	fileTransmision << "P3\n" << ancho << " " << alto << "\n255\n";

	Vector3D origen = config.camaraOrigen;
	Mapeador mapeador;

	for (int y = alto - 1; y >= 0; y--)
	{
		for (int x = 0; x < ancho; x++)
		{
			float u = ((2.0f * x / ancho) - 1.0f) * ratio;
			float v = (2.0f * y / alto) - 1.0f;

			Vector3D dir(u, v, -config.distanciaPlano);
			dir = dir.normalizado();

			Rayo rayo(origen, dir);

			Color c = mapeador.interseccion(rayo, escenario, 0);
			escribirColor(file, c);

			Interseccion inter;
			if (mapeador.obtenerInterseccion(rayo, escenario, inter)) {
				escribirGris(fileReflexion, inter.objetoIntersectado->material.kReflexion);
				escribirGris(fileTransmision, inter.objetoIntersectado->material.kRefraccion);
			}
			else {
				escribirGris(fileReflexion, 0.0f);
				escribirGris(fileTransmision, 0.0f);
			}
		}
	}

	file.close();
	fileReflexion.close();
	fileTransmision.close();
}
