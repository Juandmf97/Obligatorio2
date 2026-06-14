#include "../headers/Luz.h"

Luz::Luz(TipoLuz t, const Vector3D& o, const Vector3D& d, const Color& c, float i) : tipo(t), origen(o), direccion(d), color(c), intensidad(i) {}

Vector3D Luz::obtenerDireccion(const Vector3D& punto) const {
	if (tipo == DIRECCIONAL) {
		return direccion.normalizado();
	}
	if (tipo == PUNTUAL) {
		return (punto - origen).normalizado();
	}
	return Vector3D(0, 0, 0);
}

float Luz::obtenerDistancia(const Vector3D& punto) const {
	if (tipo == DIRECCIONAL) {
		return 1e10f;
	}
	if (tipo == PUNTUAL) {
		return (punto - origen).modulo();
	}
	return 1e10f;
}