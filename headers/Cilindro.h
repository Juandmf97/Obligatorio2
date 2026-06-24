#pragma once
#include "Objeto.h"
#include "Vector3D.h"

class Cilindro : public Objeto {
public:
    Vector3D centro; // Centro de la base del cilindro
    float radio;
    float altura;

    Cilindro();
    Cilindro(const Vector3D& c, float r, float h, const Material& mat);

    bool intersecta(const Rayo& rayo, Interseccion& inter) override;
    Vector3D normal(const Vector3D& punto) const override;
};