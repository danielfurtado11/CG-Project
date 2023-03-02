#include <math.h>
#include <vector>
#include <string.h>

#include <iostream>
#include <fstream>

#include "ponto.h"
using namespace std;

vector<Ponto> plane(float dim,float div){

    vector<Ponto> pontos;

    float dist = dim/2; // Distância do a origem até a uma aresta
    float move = dim/div; // O quanto cada divisão tem de comprimento
    float xtemp,ztemp,x,z; // Variáveis 
    z = -(dim/2);
    for(int i = 0; i < div; i++){
        x = -(dim/2);
        xtemp = x + move;
        ztemp = z + move;
        for(int j = 0; j < div; j++){
            pontos.push_back(Ponto(x,0,z));
            pontos.push_back(Ponto(xtemp,0,z));
            pontos.push_back(Ponto(x,0,ztemp));

            pontos.push_back(Ponto(xtemp,0,z));
            pontos.push_back(Ponto(xtemp,0,ztemp));
            pontos.push_back(Ponto(x,0,ztemp));
            x += move;
            xtemp += move;
        }
        z+=move;
    }
    return pontos;
}

vector<Ponto> box(float dim,float div){

    vector<Ponto> pontos;
    float dist = dim/2;
    float move = dim/div;
    float xtemp,ytemp,ztemp,x,y,z;
    
    // Faces de Cima e de Baixo
    z = -(dim/2);
    y = (dim/2);
    for(int i = 0; i < div; i++){
        x = -(dim/2);
        xtemp = x + move;
        ztemp = z + move;
        for(int j = 0; j < div; j++){
            pontos.push_back(Ponto(x,y,z));
            pontos.push_back(Ponto(xtemp,y,z));
            pontos.push_back(Ponto(x,y,ztemp));

            pontos.push_back(Ponto(xtemp,y,z));
            pontos.push_back(Ponto(xtemp,y,ztemp));
            pontos.push_back(Ponto(x,y,ztemp));

            pontos.push_back(Ponto(x,-y,z));
            pontos.push_back(Ponto(x,-y,ztemp));
            pontos.push_back(Ponto(xtemp,-y,z));

            pontos.push_back(Ponto(x,-y,ztemp));
            pontos.push_back(Ponto(xtemp,-y,ztemp));
            pontos.push_back(Ponto(xtemp,-y,z));
            x += move;
            xtemp += move;
        }
        z+=move;
    }

    // Faces da Direita e da Esquerda
    x = (dim/2);
    y = -(dim/2);
    for(int i = 0; i < div; i++){
        z = (dim/2);
        ztemp = z - move;
        ytemp = y + move;
        for(int j = 0;j < div;j++){
            pontos.push_back(Ponto(x,y,z));
            pontos.push_back(Ponto(x,y,ztemp));
            pontos.push_back(Ponto(x,ytemp,z));

            pontos.push_back(Ponto(x,y,ztemp));
            pontos.push_back(Ponto(x,ytemp,ztemp));
            pontos.push_back(Ponto(x,ytemp,z));

            pontos.push_back(Ponto(-x,y,z));
            pontos.push_back(Ponto(-x,ytemp,z));
            pontos.push_back(Ponto(-x,y,ztemp));

            pontos.push_back(Ponto(-x,y,ztemp));
            pontos.push_back(Ponto(-x,ytemp,z));
            pontos.push_back(Ponto(-x,ytemp,ztemp));

            z-=move;
            ztemp -= move;
        }
        y+=move;
    }


    // Faces da Frente e de Atrás
    z = (dim/2);
    y = -(dim/2);
    for(int i = 0; i < div; i++){
        x = -(dim/2);
        xtemp = x + move;
        ytemp = y + move;
        for(int j = 0;j < div;j++){
            pontos.push_back(Ponto(x,y,z));
            pontos.push_back(Ponto(x,y,ztemp));
            pontos.push_back(Ponto(x,ytemp,z));

            pontos.push_back(Ponto(x,y,ztemp));
            pontos.push_back(Ponto(x,ytemp,ztemp));
            pontos.push_back(Ponto(x,ytemp,z));

            pontos.push_back(Ponto(-x,y,z));
            pontos.push_back(Ponto(-x,ytemp,z));
            pontos.push_back(Ponto(-x,y,ztemp));

            pontos.push_back(Ponto(-x,y,ztemp));
            pontos.push_back(Ponto(-x,ytemp,z));
            pontos.push_back(Ponto(-x,ytemp,ztemp));

            z-=move;
            ztemp -= move;
        }
        y+=move;
    }

    return pontos;
}

void writePointsToFile(vector<Ponto> points, string filename) {
    ofstream arquivo(filename);
    //arquivo << points.size() << "\n";

    for(Ponto p : points) {
        float px = p.getX();
        float py = p.getY();
        float pz = p.getZ();

        string line = to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + "\n";
        arquivo << line;
    }

    arquivo.close();
}

int main(){
    vector<Ponto> pontos;
    pontos = plane(2,1);
    writePointsToFile(pontos,"bolacha");
    return 0;
}