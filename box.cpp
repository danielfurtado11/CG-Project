
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

void box(double dim, double div, std::string filename){

    double move;
    double x;
    double y;
    double z;
    double simetricoX;
    double simetricoY;
    double simetricoZ;
    double xtemp;
    double ytemp;
    double ztemp;
    int pontos = 0;
    std::ofstream arquivo(filename);

    move = dim/div;

    // Elaboração das Faces Frente e Trás
    z = dim/2;
    simetricoZ = -z;
    y = -(dim/2);
    for(int vertical = 0; vertical < div; vertical++){
        x = -(dim/2);
        xtemp = x + move;
        ytemp = y + move;
        for(int horizontal = 0; horizontal < div; horizontal++){

            arquivo << x << "," << y << "," << z << std::endl;
            arquivo << xtemp << "," << y << "," << z << std::endl;
            arquivo << x << "," << ytemp << "," << z << std::endl;

            arquivo << xtemp << "," << y << "," << z << std::endl;
            arquivo << xtemp << "," << ytemp << "," << z << std::endl;
            arquivo << x << "," << ytemp << "," << z << std::endl;

            arquivo << x << "," << y << "," << simetricoZ << std::endl;
            arquivo << x << "," << ytemp << "," << simetricoZ << std::endl;
            arquivo << xtemp << "," << y << "," << simetricoZ << std::endl;

            arquivo << xtemp << "," << y << "," << simetricoZ << std::endl;
            arquivo << x << "," << ytemp << "," << simetricoZ << std::endl;
            arquivo << xtemp << "," << ytemp << "," << simetricoZ << std::endl;

            x += move;
            xtemp += move;
            pontos+=4;
            }
        y += move;

    }

    // Elaboração das faces da Direita e Esquerda
    x = (dim/2);
    simetricoX = -x;
    y = -(dim/2);
    for(int vertical = 0; vertical < div; vertical++){
        z = dim/2;
        ztemp = z - move;
        ytemp = y + move;
        for(int horizontal = 0; horizontal < div; horizontal++){

            arquivo << x << "," << y << "," << z << std::endl;
            arquivo << x << "," << ytemp << "," << z << std::endl;
            arquivo << x << "," << y << "," << ztemp << std::endl;

            arquivo << x << "," << y << "," << ztemp << std::endl;
            arquivo << x << "," << ytemp << "," << z << std::endl;
            arquivo << x << "," << ytemp << "," << ztemp << std::endl;

            arquivo << simetricoX << "," << y << "," << z << std::endl;
            arquivo << simetricoX << "," << y << "," << simetricoZ << std::endl;
            arquivo << simetricoX << "," << ytemp << "," << z << std::endl;

            arquivo << simetricoX << "," << ytemp << "," << z << std::endl;
            arquivo << simetricoX << "," << y << "," << simetricoZ << std::endl;
            arquivo << simetricoX << "," << ytemp << "," << simetricoZ << std::endl;

            z -= move;
            ztemp -= move;
            pontos+=4;
        }
    }


    // Elaboração das faces de Cima e Baixo
    y = (dim/2);
    simetricoY = -y;
    z = -(dim/2);
    for(int vertical = 0; vertical < div; vertical++){
        x = -(dim/2);
        xtemp = x + move;
        ztemp = z - move;
        for(int horizontal = 0; horizontal < div; horizontal++){
            
            arquivo << x << "," << y << "," << z << std::endl;
            arquivo << xtemp << "," << y << "," << z << std::endl;
            arquivo << x << "," << y << "," << ztemp << std::endl;

            arquivo << xtemp << "," << y << "," << z << std::endl;
            arquivo << xtemp << "," << y << "," << ztemp << std::endl;
            arquivo << x << "," << y << "," << ztemp << std::endl;

            arquivo << x << "," << simetricoY  << "," << z << std::endl;
            arquivo << x << "," << simetricoY << "," << ztemp << std::endl;
            arquivo << xtemp << "," << simetricoY  << "," << z << std::endl;

            arquivo << xtemp << "," << simetricoY  << "," << z << std::endl;
            arquivo << x << "," << simetricoY  << "," << ztemp << std::endl;
            arquivo << xtemp << "," << simetricoY  << "," << ztemp << std::endl;

            x += move;
            xtemp += move;
            pontos+=4;
        }
    }

    arquivo << "NUMERO DE PONTOS" << pontos << std::endl;
}

int main() {
    box(4,2,"box.3d");
    return 0;
}