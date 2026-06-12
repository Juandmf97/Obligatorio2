#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <fstream>

#include "../headers/Escenario.h"
#include "../headers/Color.h"
#include "../headers/Mapeador.h"

using namespace std;

void render(const Escenario& escenario)
{
	int ancho = 800;
	int alto = 600;
	float ratio = (float)ancho / (float)alto;

	std::ofstream file("C:\\Users\\Usuario\\Desktop\\imagen.ppm");

	file << "P3\n" << ancho << " " << alto << "\n255\n";

	Vector3D origen(0, 0, 0);

	for (int y = alto - 1; y >= 0; y--)
	{
		for (int x = 0; x < ancho; x++)
		{
			float u = ((2.0f * x / ancho) - 1.0f) * ratio;
			float v = (2.0f * y / alto) - 1.0f;

			Vector3D dir(u, v, -1);
			dir = dir.normalizado();

			Rayo rayo(origen, dir);

			Mapeador mapeador;

			Color c = mapeador.interseccion(rayo, escenario);

			file << (int)(c.r * 255) << " "
				<< (int)(c.g * 255) << " "
				<< (int)(c.b * 255) << "\n";
		}
	}

	file.close();
}

int main(int argc, char *argv[]) {
	Escenario escena;

	escena.crearEsfera(Vector3D(0, 0, -5), 1.0f, Color(0, 1, 0));
	escena.agregarLuz(Vector3D(0, -1, 0), Color(1, 1, 1));
	escena.crearPlano(Vector3D(0, -1, 0), Vector3D(0, 1, 0), Color(1, 0, 0));

	render(escena);
	return 0;
}
