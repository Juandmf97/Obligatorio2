#pragma once

#include "Interseccion.h"
#include "Escenario.h"
#include "Rayo.h"
#include "Color.h"

class Mapeador {
public :
	float atenuar(float d);
	bool obtenerInterseccion(const Rayo& rayo, const Escenario& escenario, Interseccion& inter);
	bool estaSombreado(const Escenario& escenario, const Interseccion& inter, const Luz* luz);
	Color sombrear(const Luz* luz, const Interseccion& inter, const bool sombreado, const Rayo& rayoIncidente);
	Color calcularIluminacion(const Color& luzDifusa, const Interseccion& inter, const Escenario& escenario);
	Color calcularReflexion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad);
	Color calcularRefraccion(const Rayo& rayoIncidente, const Interseccion& inter, const Escenario& escenario, const int profundidad);
	Color calcularColor(const Interseccion& inter, const Color& colorReflejado, const Color& colorDifuso, const Color& colorRefractado);

	Color interseccion(const Rayo& rayo, const Escenario& escenario, int profundidad);
};