#pragma once

#include "CargadorXML.h"
#include "Escenario.h"

class Renderizador {
public:
	void render(const Escenario& escenario, const ConfigRender& config);
};
