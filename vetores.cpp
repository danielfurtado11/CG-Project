#include <cmath>

#include "vetores.h"

Ponto normalizarPonto(Ponto p){
    float x = p.getX();
    float y = p.getY();
    float z = p.getZ();

    float aux = sqrt(x * x + y * y + z * z);
    x = x / aux;
    y = y / aux;
    z = z /aux;

    return Ponto(x,y,z);
}

Ponto sub_ponto(Ponto p1, Ponto p2) {

    return Ponto(p1.getX() - p2.getX(),
                 p1.getY() - p2.getY(),
                 p1.getZ() - p2.getZ());
}

void normalizarVector(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

