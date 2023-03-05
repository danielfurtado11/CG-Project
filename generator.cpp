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


    	// Faces da Frente e de Trás
	z = (dim/2);
	y = -(dim/2);
	for(int i = 0; i < div; i++){
		x = -(dim/2);
		xtemp = x + move;
		ytemp = y + move;
		for(int j = 0; j < div; j++){
			pontos.push_back(Ponto(x,ytemp,z));
            pontos.push_back(Ponto(x,y,z));
            pontos.push_back(Ponto(xtemp,y,z));

            pontos.push_back(Ponto(xtemp,y,z));
            pontos.push_back(Ponto(xtemp,ytemp,z));
            pontos.push_back(Ponto(x,ytemp,z));

            pontos.push_back(Ponto(xtemp,y,-z));
            pontos.push_back(Ponto(x,y,-z));
            pontos.push_back(Ponto(x,ytemp,-z));

            pontos.push_back(Ponto(x,ytemp,-z));
            pontos.push_back(Ponto(xtemp,ytemp,-z));
            pontos.push_back(Ponto(xtemp,y,-z));

        	x+=move;
        	xtemp+=move;
    	}
    	y+=move;
	}

    return pontos;
}

vector <Ponto> cone(float raio, float altura, int slices, int stacks){
    vector<Ponto> pontos;
    float angulo = (2*M_PI)/slices;
    altura = altura / stacks;
    for(int j = 0; j < stacks; j++){        //stacks
        
        float raioBaixo = (stacks - j)*(raio/stacks);
        float raioCima = (stacks - (j + 1))*(raio/stacks);
        
        for(int i = 0; i < slices;i++){        //slices

            float angulo_inicial = i * angulo;
            float angulo_final = angulo_inicial + angulo;

            //triangulo com base em baixo, apontado para cima
            pontos.push_back(Ponto(sin(angulo_final)*raioCima, (j+1)*altura, cos(angulo_final)*raioCima));
            pontos.push_back(Ponto(sin(angulo_inicial)*raioBaixo, j*altura, cos(angulo_inicial)*raioBaixo));
            pontos.push_back(Ponto(sin(angulo_final)*raioBaixo, j*altura, cos(angulo_final)*raioBaixo));
    
            if(j != stacks){
                pontos.push_back(Ponto(sin(angulo_inicial)*raioBaixo, j*altura, cos(angulo_inicial)*raioBaixo));
                pontos.push_back(Ponto(sin(angulo_final)*raioCima, (j+1)*altura, cos(angulo_final)*raioCima));
                pontos.push_back(Ponto(sin(angulo_inicial)*raioCima, (j+1)*altura, cos(angulo_inicial)*raioCima));
            }

            //base de baixo
            pontos.push_back(Ponto(0,j*altura,0));
            pontos.push_back(Ponto(sin(angulo_final)*raioBaixo, j*altura, cos(angulo_final)*raioBaixo));
            pontos.push_back(Ponto(sin(angulo_inicial)*raioBaixo, j*altura, cos(angulo_inicial)*raioBaixo));
        }
    }    

    return pontos;
}

vector<Ponto> sphere(float raio, int slices, int stacks){
    vector<Ponto> pontos;

	float angulo_default_slice = (2 * M_PI) / slices;
	float angulo_default_stack = M_PI / stacks;

	for (int i = 0; i < slices; i++) {

		float aSlice = i * angulo_default_slice;
		float temp_aSlice = aSlice + angulo_default_slice;

		float aStack = - M_PI_2;
		float temp_aStack = aStack + angulo_default_stack;
		
		for (int j = 0; j < stacks; j++){
			if(j == 0){
                pontos.push_back(Ponto(0.0, -raio, 0.0));
                pontos.push_back(Ponto(raio * cos(temp_aStack) * sin(temp_aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(temp_aSlice)));
                pontos.push_back(Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin (temp_aStack) , raio * cos(temp_aStack) * cos(aSlice)));

			}else if(j == stacks-1){
                pontos.push_back(Ponto(0.0, raio, 0.0));
                pontos.push_back(Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice)));
                pontos.push_back(Ponto(raio * cos(aStack) * sin(aSlice), raio * sin(aStack), raio * cos(aStack) * cos(aSlice)));
				
			}else{
                pontos.push_back(Ponto(raio * cos(aStack) * sin(aSlice), raio * sin(aStack), raio * cos(aStack) * cos(aSlice)));
                pontos.push_back(Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice)));
                pontos.push_back(Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(aSlice)));

                pontos.push_back(Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice)));
                pontos.push_back(Ponto(raio * cos(temp_aStack) * sin(temp_aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(temp_aSlice)));
                pontos.push_back(Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(aSlice)));
            }  
			aStack += angulo_default_stack;
			temp_aStack += angulo_default_stack;
		}
	}
	return pontos;
}


void writePointsToFile(vector<Ponto> points, string filename) {
    ofstream arquivo(filename);
    arquivo << points.size() << "\n";

    for(Ponto p : points) {
        float px = p.getX();
        float py = p.getY();
        float pz = p.getZ();

        string line = to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + "\n";
        arquivo << line;
    }

    arquivo.close();
}

int main(int argc, char** argv){
    vector<Ponto> pontos;

    if (strcmp(argv[1],"plane") == 0){
        pontos = plane(stof(argv[2]), stof(argv[3]));
        writePointsToFile(pontos,argv[4]);
    }
    else if (strcmp(argv[1],"box") == 0){
        pontos = box(stof(argv[2]), stof(argv[3]));
        writePointsToFile(pontos, argv[4]);
    }
    else if (strcmp(argv[1], "cone") == 0){
        pontos = cone(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
        writePointsToFile(pontos, argv[6]);
    }
    else if (strcmp(argv[1], "sphere") == 0){
        pontos = sphere(stof(argv[2]), stof(argv[3]), stof(argv[4]));
        writePointsToFile(pontos, argv[5]);
    }
    else printf("Erro no input");
    
    return 0;
}