#pragma once

#include "Escenario.h"
#include "Rayo.h"
#include "Color.h"

class Mapeador {
public :
	Color interseccion(const Rayo& rayo, const Escenario& escenario);
};