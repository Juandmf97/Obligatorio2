#include "../headers/Cilindro.h"
#include <cmath>

Cilindro::Cilindro() : centro(), radio(0.0f), altura(0.0f) {}

Cilindro::Cilindro(const Vector3D& c, float r, float h, const Material& mat)
    : Objeto(mat), centro(c), radio(r), altura(h) {
}

bool Cilindro::intersecta(const Rayo& rayo, Interseccion& inter) {
    inter.alfa = 1e20f; // Inicializamos con un valor infinito (ver esto q tal)
    bool hayInterseccion = false;

    // --- 1. INTERSECCIÓN CON EL CUERPO VERTICAL (TUBO) ---
    float ox = rayo.origen.x - centro.x;
    float oz = rayo.origen.z - centro.z;
    float dx = rayo.direccion.x;
    float dz = rayo.direccion.z;

    float a = dx * dx + dz * dz;
    float b = 2.0f * (ox * dx + oz * dz);
    float c = (ox * ox + oz * oz) - (radio * radio);

    float delta = b * b - 4.0f * a * c;

    if (delta >= 0.0f) {
        float sqrt_delta = sqrt(delta);
        float alfa1 = (-b - sqrt_delta) / (2.0f * a);
        float alfa2 = (-b + sqrt_delta) / (2.0f * a);

        // Chequear el primer punto de impacto en el tubo
        if (alfa1 > 0.001f) {
            float y_impacto = rayo.origen.y + rayo.direccion.y * alfa1;
            // Verificamos si cae dentro del rango de la altura en Y
            if (y_impacto >= centro.y && y_impacto <= (centro.y + altura)) {
                inter.alfa = alfa1;
                hayInterseccion = true;
            }
        }
        // Si alfa1 no sirvió, chequear alfa2
        if (!hayInterseccion && alfa2 > 0.001f) {
            float y_impacto = rayo.origen.y + rayo.direccion.y * alfa2;
            if (y_impacto >= centro.y && y_impacto <= (centro.y + altura)) {
                inter.alfa = alfa2;
                hayInterseccion = true;
            }
        }
    }

    // --- 2. INTERSECCIÓN CON LAS TAPAS (DISCOS HORIZONTALES) ---
    if (std::abs(rayo.direccion.y) > 1e-6f) {

        // Tapa Inferior (no deberia estar en ningun lugar)
        float alfaBase = (centro.y - rayo.origen.y) / rayo.direccion.y;
        if (alfaBase > 0.001f && alfaBase < inter.alfa) {
            float px = rayo.origen.x + rayo.direccion.x * alfaBase;
            float pz = rayo.origen.z + rayo.direccion.z * alfaBase;
            // Distancia en el plano XZ al centro del cilindro
            float distCuadrado = (px - centro.x) * (px - centro.x) + (pz - centro.z) * (pz - centro.z);
            if (distCuadrado <= (radio * radio)) {
                inter.alfa = alfaBase;
                hayInterseccion = true;
            }
        }

        // Tapa Superior 
        float yTop = centro.y + altura;
        float alfaTop = (yTop - rayo.origen.y) / rayo.direccion.y;
        if (alfaTop > 0.001f && alfaTop < inter.alfa) {
            float px = rayo.origen.x + rayo.direccion.x * alfaTop;
            float pz = rayo.origen.z + rayo.direccion.z * alfaTop;
            float distCuadrado = (px - centro.x) * (px - centro.x) + (pz - centro.z) * (pz - centro.z);
            if (distCuadrado <= (radio * radio)) {
                inter.alfa = alfaTop;
                hayInterseccion = true;
            }
        }
    }
    if (hayInterseccion) {
        inter.objetoIntersectado = this;
        inter.puntoInterseccion = rayo.origen + rayo.direccion * inter.alfa;
        inter.normal = this->normal(inter.puntoInterseccion);
    }
    return hayInterseccion;
}

Vector3D Cilindro::normal(const Vector3D& punto) const {
    // Si el punto está en la tapa inferior (Y es igual al centro Y)
    if (std::abs(punto.y - centro.y) < 0.001f) {
        return Vector3D(0.0f, -1.0f, 0.0f); // Normal hacia abajo
    }

    if (std::abs(punto.y - (centro.y + altura)) < 0.001f) {
        return Vector3D(0.0f, 1.0f, 0.0f); // Normal hacia arriba
    }

    Vector3D normalTubo(punto.x - centro.x, 0.0f, punto.z - centro.z);
    return normalTubo.normalizado();
}