#include <string>
#include <sstream>
#include <vector>

#include "ponto.h"
using namespace std;


// Construtor do Ponto
Ponto::Ponto(float x,float y,float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

float Ponto::getX(){
    return this->x;
}

float Ponto::getY(){
    return this->y;
}

float Ponto::getZ(){
    return this->z;
}
