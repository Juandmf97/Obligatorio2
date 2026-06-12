#include "../headers/Rayo.h"

Rayo::Rayo() : origen(), direccion() {}

Rayo::Rayo(const Vector3D& o, const Vector3D& d) : origen(o), direccion(d.normalizado()) {}