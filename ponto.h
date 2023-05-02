#ifndef PONTO_H
#define PONTO_H
#include <string>

using namespace std;

class Ponto{
    private:
        float x,y,z;
    public:
        Ponto(float x,float y,float z);
        Ponto(string line);
        float getX();
        float getY();
        float getZ();
};

#endif 