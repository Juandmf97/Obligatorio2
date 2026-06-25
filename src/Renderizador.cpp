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
	std::ofstream fileAmbiente(agregarSufijo(config.salida, "_ambiente"));
	std::ofstream fileDifusa(agregarSufijo(config.salida, "_difusa"));
	std::ofstream filePhong(agregarSufijo(config.salida, "_phong"));

	std::ofstream fileReflexion(agregarSufijo(config.salida, "_reflexion"));
	std::ofstream fileTransmision(agregarSufijo(config.salida, "_transmision"));

	file << "P3\n" << ancho << " " << alto << "\n255\n";
	fileAmbiente << "P3\n" << ancho << " " << alto << "\n255\n";
	fileDifusa << "P3\n" << ancho << " " << alto << "\n255\n";
	filePhong << "P3\n" << ancho << " " << alto << "\n255\n";

	fileReflexion << "P3\n" << ancho << " " << alto << "\n255\n";
	fileTransmision << "P3\n" << ancho << " " << alto << "\n255\n";

	Vector3D origen = config.camaraOrigen;
	Vector3D frente = (config.camaraObjetivo - origen).normalizado();
	Vector3D derecha = frente.cruz(config.camaraArriba).normalizado();
	Vector3D arriba = derecha.cruz(frente).normalizado();
	Mapeador mapeador;
	int N = config.factorAA;
	int totalDivisiones = N * N;

	for (int y = alto - 1; y >= 0; y--)
	{
		for (int x = 0; x < ancho; x++)
		{
			Color colorFinal(0, 0, 0);
			Color colorAmbiente(0, 0, 0);
			Color colorDifuso(0, 0, 0);
			Color colorPhong(0, 0, 0);
			float reflexionAcumulada = 0.0f;
			float transmisionAcumulada = 0.0f;

			for (int sy = 0; sy < N; sy++) {
				for (int sx = 0; sx < N; sx++) {
					float offsetX = (sx + 0.5f) / N;
					float offsetY = (sy + 0.5f) / N;
					float u = ((2.0f * (x + offsetX) / ancho) - 1.0f) * ratio;
					float v = (2.0f * (y + offsetY) / alto) - 1.0f;
					Vector3D dir = derecha * u + arriba * v + frente * config.distanciaPlano;
					dir = dir.normalizado();
					Rayo rayo(origen, dir);
					Color colorAmbienteIteracion(0, 0, 0);
					Color colorDifusoIteracion(0, 0, 0);
					Color colorPhongIteracion(0, 0, 0);
					Color c = mapeador.interseccion(rayo, escenario, 0, colorAmbienteIteracion, colorDifusoIteracion, colorPhongIteracion);
					colorFinal += c;
					colorAmbiente += colorAmbienteIteracion;
					colorDifuso += colorDifusoIteracion;
					colorPhong += colorPhongIteracion;

					Interseccion inter;
					if (mapeador.obtenerInterseccion(rayo, escenario, inter)) {
						reflexionAcumulada += inter.objetoIntersectado->material.kReflexion;
						transmisionAcumulada += inter.objetoIntersectado->material.kRefraccion;
					}
				}
			}

			escribirColor(file, colorFinal / totalDivisiones);
			escribirColor(fileAmbiente, colorAmbiente / totalDivisiones);
			escribirColor(fileDifusa, colorDifuso / totalDivisiones);
			escribirColor(filePhong, colorPhong / totalDivisiones);
			escribirGris(fileReflexion, reflexionAcumulada / totalDivisiones);
			escribirGris(fileTransmision, transmisionAcumulada / totalDivisiones);
		}
	}

	file.close();
	fileAmbiente.close();
	fileDifusa.close();
	filePhong.close();
	fileReflexion.close();
	fileTransmision.close();
}
