#include "../headers/CargadorXML.h"

#include "../headers/Material.h"
#include "../tinyxml2/tinyxml2.h"

#include <iostream>
#include <map>
#include <sstream>

namespace {
	float obtenerFloat(const tinyxml2::XMLElement* elemento, const char* nombre, float valorPorDefecto) {
		float valor = valorPorDefecto;
		elemento->QueryFloatAttribute(nombre, &valor);
		return valor;
	}

	int obtenerInt(const tinyxml2::XMLElement* elemento, const char* nombre, int valorPorDefecto) {
		int valor = valorPorDefecto;
		elemento->QueryIntAttribute(nombre, &valor);
		return valor;
	}

	std::string obtenerTexto(const tinyxml2::XMLElement* elemento, const char* nombre, const std::string& valorPorDefecto = "") {
		const char* valor = elemento->Attribute(nombre);
		return valor != nullptr ? valor : valorPorDefecto;
	}

	Vector3D parsearVector(const char* texto, const Vector3D& valorPorDefecto = Vector3D()) {
		if (texto == nullptr) {
			return valorPorDefecto;
		}

		std::string limpio = texto;
		for (char& c : limpio) {
			if (c == ',' || c == ';') {
				c = ' ';
			}
		}

		std::stringstream ss(limpio);
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		if (!(ss >> x >> y >> z)) {
			return valorPorDefecto;
		}

		return Vector3D(x, y, z);
	}

	Vector3D obtenerVector(const tinyxml2::XMLElement* elemento, const char* nombre, const Vector3D& valorPorDefecto = Vector3D()) {
		return parsearVector(elemento->Attribute(nombre), valorPorDefecto);
	}

	TipoLuz obtenerTipoLuz(const std::string& texto) {
		if (texto == "direccional" || texto == "DIRECCIONAL") {
			return DIRECCIONAL;
		}
		return PUNTUAL;
	}

	bool buscarMaterial(const std::map<std::string, Material>& materiales, const std::string& id, Material& material) {
		auto it = materiales.find(id);
		if (it == materiales.end()) {
			return false;
		}

		material = it->second;
		return true;
	}
}

bool CargadorXML::cargar(const std::string& ruta, Escenario& escenario, ConfigRender& config) {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(ruta.c_str());

	if (error != tinyxml2::XML_SUCCESS) {
		std::cerr << "No se pudo cargar la escena XML: " << ruta << std::endl;
		std::cerr << doc.ErrorStr() << std::endl;
		return false;
	}

	tinyxml2::XMLElement* raiz = doc.FirstChildElement("escena");
	if (raiz == nullptr) {
		std::cerr << "El XML no tiene un nodo raiz <escena>." << std::endl;
		return false;
	}

	std::map<std::string, Material> materiales;
	raiz->QueryFloatAttribute("luzAmbiente", &escenario.luzAmbiente);

	if (tinyxml2::XMLElement* render = raiz->FirstChildElement("render")) {
		config.ancho = obtenerInt(render, "ancho", config.ancho);
		config.alto = obtenerInt(render, "alto", config.alto);
		config.salida = obtenerTexto(render, "salida", config.salida);
		config.factorAA = obtenerInt(render, "factorAA", config.factorAA);
	}

	if (tinyxml2::XMLElement* camara = raiz->FirstChildElement("camara")) {
		config.camaraOrigen = obtenerVector(camara, "origen", config.camaraOrigen);
		config.distanciaPlano = obtenerFloat(camara, "distanciaPlano", config.distanciaPlano);
	}

	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("material"); elemento != nullptr; elemento = elemento->NextSiblingElement("material")) {
		std::string id = obtenerTexto(elemento, "id");
		if (id.empty()) {
			std::cerr << "Material sin id en XML." << std::endl;
			return false;
		}

		Color color(
			obtenerFloat(elemento, "r", 1.0f),
			obtenerFloat(elemento, "g", 1.0f),
			obtenerFloat(elemento, "b", 1.0f)
		);

		materiales[id] = Material(
			color,
			obtenerFloat(elemento, "ka", 0.0f),
			obtenerFloat(elemento, "kd", 0.0f),
			obtenerFloat(elemento, "kr", 0.0f),
			obtenerFloat(elemento, "kt", 0.0f),
			obtenerFloat(elemento, "ks", 0.0f),
			obtenerFloat(elemento, "phong", 1.0f),
			obtenerFloat(elemento, "ior", 1.0f)
		);
	}

	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("luz"); elemento != nullptr; elemento = elemento->NextSiblingElement("luz")) {
		escenario.agregarLuz(
			obtenerTipoLuz(obtenerTexto(elemento, "tipo", "puntual")),
			obtenerVector(elemento, "origen"),
			obtenerVector(elemento, "direccion", Vector3D(0, -1, 0)),
			Color(
				obtenerFloat(elemento, "r", 1.0f),
				obtenerFloat(elemento, "g", 1.0f),
				obtenerFloat(elemento, "b", 1.0f)
			),
			obtenerFloat(elemento, "intensidad", 1.0f)
		);
	}

	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("esfera"); elemento != nullptr; elemento = elemento->NextSiblingElement("esfera")) {
		std::string idMaterial = obtenerTexto(elemento, "material");
		Material material;

		if (!buscarMaterial(materiales, idMaterial, material)) {
			std::cerr << "No existe el material de esfera: " << idMaterial << std::endl;
			return false;
		}

		escenario.crearEsfera(
			obtenerVector(elemento, "centro"),
			obtenerFloat(elemento, "radio", 1.0f),
			material
		);
	}

	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("cilindro"); elemento != nullptr; elemento = elemento->NextSiblingElement("cilindro")) {
		std::string idMaterial = obtenerTexto(elemento, "material");
		Material material;

		if (!buscarMaterial(materiales, idMaterial, material)) {
			std::cerr << "No existe el material de cilindro: " << idMaterial << std::endl;
			return false;
		}

		escenario.crearCilindro(
			obtenerVector(elemento, "centro"),
			obtenerFloat(elemento, "radio", 1.0f),
			obtenerFloat(elemento, "altura", 1.0f),
			material
		);
	}

	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("plano"); elemento != nullptr; elemento = elemento->NextSiblingElement("plano")) {
		std::string idMaterial = obtenerTexto(elemento, "material");
		Material material;

		if (!buscarMaterial(materiales, idMaterial, material)) {
			std::cerr << "No existe el material de plano: " << idMaterial << std::endl;
			return false;
		}

		escenario.crearPlano(
			obtenerVector(elemento, "punto"),
			obtenerVector(elemento, "normal", Vector3D(0, 1, 0)),
			material
		);
	}


	for (tinyxml2::XMLElement* elemento = raiz->FirstChildElement("prisma"); elemento != nullptr; elemento = elemento->NextSiblingElement("prisma")) {
		std::string idMaterial = obtenerTexto(elemento, "material");
		Material material;

		if (!buscarMaterial(materiales, idMaterial, material)) {
			std::cerr << "No existe el material de prisma: " << idMaterial << std::endl;
			return false;
		}

		escenario.crearPrisma(
			obtenerVector(elemento, "punto1"),
			obtenerVector(elemento, "punto2"),
			material
		);
	}

	return true;
}
