#include <math.h>
#include <vector>
#include <string.h>
#include <map>
#include <sstream>


#include <iostream>
#include <fstream>

#include "ponto.h"
#include "Matrix.tpp"
#include "vetores.h"
using namespace std;


// Function that calculates the normal in p1, given two other points, following the right hand rule
Ponto calculaNormal(Ponto p1, Ponto p2, Ponto p3) {
	Ponto v1 = sub_ponto(p2, p1);
	Ponto v2 = sub_ponto(p3, p1);
	Ponto n = cross_ponto(v1, v2);

	return normalizarPonto(n);
}


void bezierFile (string patchFile, int tess_level,vector<Ponto>* p, vector<Ponto>* n){

    int i = 0;
    string linha_auxiliar; // linha auxiliar que vamos usar para varrer o ficheiro
    ifstream ficheiro;  // ficheiro 


    vector<Ponto> pontos_de_controlo;
    int nr_pontos_controlo;
    map<int, vector<int>> patches; 
    int nr_patches;

    ficheiro.open(patchFile.c_str(), ios::in); // Abre o patchFile em modo de leitura;

    if(ficheiro.is_open()){

        getline(ficheiro, linha_auxiliar);
        nr_patches = atoi(linha_auxiliar.c_str()); // Guardamos o número de patches, no ficheiro corresponde ao 32

        while(i < nr_patches){
            getline(ficheiro, linha_auxiliar); // Lẽ a linha

            vector<int> patch;

            string token;
            istringstream tokenStream(linha_auxiliar); // Isto vai permitir que a linha_auxiliar seja dividida por virgulas

    		while (getline(tokenStream, token, ',')) {
                // Fazemos a divisão por virgula
				int index = atoi(token.c_str());
                // Transformamos o valor em int
                patch.push_back(index);
                // Mandamos o valor para o vector
			}

            patches[i] = patch; // No mapa fazemos corresponder com a key que é o número de patch que esatamos
            i++;

        }

        i = 0;
        // Lemos a linha_auxiliar para guardar o número de pontos de controlo
        getline(ficheiro, linha_auxiliar);
        nr_pontos_controlo = atoi(linha_auxiliar.c_str());

        while( i < nr_pontos_controlo){
            // Vamos ler cada linha
            getline(ficheiro,linha_auxiliar);

            istringstream tokenLinha(linha_auxiliar);
            string varrer;
            float x, y, z;

            // Extrair os valores x, y e z a partir da linha
            if (getline(tokenLinha, varrer, ',')) {
                x = stof(varrer);
            }
            if (getline(tokenLinha, varrer, ',')) {
                y = std::stof(varrer);
            }
            if (getline(tokenLinha, varrer, ',')) {
                z = std::stof(varrer);
            }

            // Passamos para ponto
            Ponto p = Ponto(x,y,z);
            // Adicionamos o ponto ao vector
            pontos_de_controlo.push_back(p);
            i++;
        }

        ficheiro.close();

    }else{
            std::cout << "[ERRO] : Não foi possível abrir o ficheiro: " << patchFile.c_str() << "\n";
    }

    // Calculate Bezier Surfaces
    vector<Ponto> r_pontos;
    vector<Ponto> r_normais;

    // Processing of each patch
    for (int i_p = 0; i_p < nr_patches; i_p++) {
        vector<int> patch = patches[i_p];
        vector<Ponto> cps;          // patch control points
        vector<Ponto> grid_points;  // points obtained from control points

        // Get control points for this patch
        for (int i_pp = 0; i_pp < patch.size(); i_pp++) {
            cps.push_back(pontos_de_controlo[patch[i_pp]]);
        }

        // Initialize matrices to calculate bezier surface points
        Matrix<float> m = {{-1.0f,  3.0f, -3.0f,  1.0f},
                           { 3.0f, -6.0f,  3.0f,  0.0f},
                           {-3.0f,  3.0f,  0.0f,  0.0f},
                           { 1.0f,  0.0f,  0.0f,  0.0f}};

        Matrix<float> m_px = {{cps[0].getX(), cps[1].getX(), cps[2].getX(), cps[3].getX()},
                              {cps[4].getX(), cps[5].getX(), cps[6].getX(), cps[7].getX()},
                              {cps[8].getX(), cps[9].getX(), cps[10].getX(), cps[11].getX()},
                              {cps[12].getX(), cps[13].getX(), cps[14].getX(), cps[15].getX()}};

        Matrix<float> m_py = {{cps[0].getY(), cps[1].getY(), cps[2].getY(), cps[3].getY()},
                              {cps[4].getY(), cps[5].getY(), cps[6].getY(), cps[7].getY()},
                              {cps[8].getY(), cps[9].getY(), cps[10].getY(), cps[11].getY()},
                              {cps[12].getY(), cps[13].getY(), cps[14].getY(), cps[15].getY()}};

        Matrix<float> m_pz = {{cps[0].getZ(), cps[1].getZ(), cps[2].getZ(), cps[3].getZ()},
                              {cps[4].getZ(), cps[5].getZ(), cps[6].getZ(), cps[7].getZ()},
                              {cps[8].getZ(), cps[9].getZ(), cps[10].getZ(), cps[11].getZ()},
                              {cps[12].getZ(), cps[13].getZ(), cps[14].getZ(), cps[15].getZ()}};

        Matrix<float> m_x = m * m_px * m;
        Matrix<float> m_y = m * m_py * m ;
        Matrix<float> m_z = m * m_pz * m;
        

        // Give values to u and v to calculate grid points
        float inc = 1.0 / tess_level;
        for (float v = 0.0; v <= 1.0; v += inc) {
            for (float u = 0.0; u <= 1.0; u += inc) {
                Matrix<float> m_u = {{u*u*u, u*u, u, 1.0f}};

                Matrix<float> m_v = {{v*v*v},
                                     {v*v},
                                     {v},
                                     {1.0f}};
                
                float x_value = (m_u * m_x * m_v)[0][0];
                float y_value = (m_u * m_y * m_v)[0][0];
                float z_value = (m_u * m_z * m_v)[0][0];

                grid_points.push_back(Ponto(x_value, y_value, z_value));
            }
        }


        // Write triangles correspondent to the grid
        for (int v_ind = 0; v_ind < tess_level; v_ind++) {
            for (int u_ind = 0; u_ind < tess_level; u_ind++) {
                int points_per_line = tess_level + 1;
                Ponto p0 = grid_points[u_ind   + v_ind     * points_per_line];
                Ponto p1 = grid_points[u_ind+1 + v_ind     * points_per_line];
                Ponto p2 = grid_points[u_ind   + (v_ind+1) * points_per_line];
                Ponto p3 = grid_points[u_ind+1 + (v_ind+1) * points_per_line];

                r_pontos.push_back(p0); r_pontos.push_back(p2); r_pontos.push_back(p1);
                r_pontos.push_back(p1); r_pontos.push_back(p2); r_pontos.push_back(p3);

                r_normais.push_back(calculaNormal(p0, p2, p1));
                r_normais.push_back(calculaNormal(p2, p1, p0));
                r_normais.push_back(calculaNormal(p1, p0, p2));

                r_normais.push_back(calculaNormal(p1, p2, p3));
                r_normais.push_back(calculaNormal(p2, p3, p1));
                r_normais.push_back(calculaNormal(p3, p1, p2));
            }
        }

    }

    *p = r_pontos;
    *n = r_normais;

}

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
    float alturaStack = altura / stacks;
    for(int j = 0; j < stacks; j++){
        
        float raioBot = (stacks - j)*(raio/stacks);
        float raioUp = (stacks - (j + 1))*(raio/stacks);
        for(int i = 0; i < slices;i++){

            float angulo_inicial = i * angulo;
            float angulo_final = angulo_inicial + angulo;


            pontos.push_back(Ponto(sin(angulo_final)*raioUp, (j+1)*alturaStack, cos(angulo_final)*raioUp));
            pontos.push_back(Ponto(sin(angulo_inicial)*raioBot, j*alturaStack, cos(angulo_inicial)*raioBot));
            pontos.push_back(Ponto(sin(angulo_final)*raioBot, j*alturaStack, cos(angulo_final)*raioBot));
    
            if(j != stacks){
                pontos.push_back(Ponto(sin(angulo_inicial)*raioBot, j*alturaStack, cos(angulo_inicial)*raioBot));
                pontos.push_back(Ponto(sin(angulo_final)*raioUp, (j+1)*alturaStack, cos(angulo_final)*raioUp));
                pontos.push_back(Ponto(sin(angulo_inicial)*raioUp, (j+1)*alturaStack, cos(angulo_inicial)*raioUp));
            }


            pontos.push_back(Ponto(0,j*alturaStack,0));
            pontos.push_back(Ponto(sin(angulo_final)*raioBot, j*alturaStack, cos(angulo_final)*raioBot));
            pontos.push_back(Ponto(sin(angulo_inicial)*raioBot, j*alturaStack, cos(angulo_inicial)*raioBot));
        }
    }    

    return pontos;
}

void sphere(float raio, int slices, int stacks,vector <Ponto>*p, vector <Ponto>* n, vector<float>* text){
    vector<Ponto> pontos;
    vector<Ponto> normais;
    vector<float> texturas;

	float angulo_default_slice = (2 * M_PI) / slices;
	float angulo_default_stack = M_PI / stacks;

	for (int i = 0; i < slices; i++) {

		float aSlice = i * angulo_default_slice;
		float temp_aSlice = aSlice + angulo_default_slice;

		float aStack = - M_PI_2;
		float temp_aStack = aStack + angulo_default_stack;
		
		for (int j = 0; j < stacks; j++){
			if(j == 0){
                Ponto ponto_1 = Ponto(0.0f, -raio, 0.0f);
                Ponto ponto_2 = Ponto(raio * cos(temp_aStack) * sin(temp_aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(temp_aSlice));
                Ponto ponto_3 = Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin (temp_aStack) , raio * cos(temp_aStack) * cos(aSlice));
                pontos.push_back(ponto_1);
                pontos.push_back(ponto_2);
                pontos.push_back(ponto_3);
                normais.push_back(normalizarPonto(ponto_1));
                normais.push_back(normalizarPonto(ponto_2));
                normais.push_back(normalizarPonto(ponto_3));
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back(0.0);
                texturas.push_back(temp_aSlice / (2*M_PI));
                texturas.push_back((temp_aStack + M_PI_2)/M_PI);
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back((temp_aStack + M_PI_2)/M_PI);

			}else if(j == stacks-1){
                Ponto ponto_3 = Ponto(0.0f, raio, 0.0f);
                Ponto ponto_2 = Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice));
                Ponto ponto_1 = Ponto(raio * cos(aStack) * sin(aSlice), raio * sin(aStack), raio * cos(aStack) * cos(aSlice));
                pontos.push_back(ponto_1);
                pontos.push_back(ponto_2);
                pontos.push_back(ponto_3);
                normais.push_back(normalizarPonto(ponto_1));
                normais.push_back(normalizarPonto(ponto_2));
                normais.push_back(normalizarPonto(ponto_3));
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back(1.0);
                texturas.push_back(temp_aSlice / (2*M_PI));
                texturas.push_back((aStack + M_PI_2)/M_PI);
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back((aStack + M_PI_2)/M_PI);
				
			}else{
                Ponto ponto_1 = Ponto(raio * cos(aStack) * sin(aSlice), raio * sin(aStack), raio * cos(aStack) * cos(aSlice));
                Ponto ponto_2 = Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice));
                Ponto ponto_3 = Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(aSlice));
                pontos.push_back(ponto_1);
                pontos.push_back(ponto_2);
                pontos.push_back(ponto_3);
                normais.push_back(normalizarPonto(ponto_1));
                normais.push_back(normalizarPonto(ponto_2));
                normais.push_back(normalizarPonto(ponto_3));
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back((aStack + M_PI_2)/ M_PI);
                texturas.push_back(temp_aSlice / (2*M_PI));
                texturas.push_back((aStack + M_PI_2)/ M_PI);
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back((temp_aStack + M_PI_2)/ M_PI);

                Ponto ponto_4 = Ponto(raio * cos(aStack) * sin(temp_aSlice), raio * sin(aStack), raio * cos(aStack) * cos(temp_aSlice));
                Ponto ponto_5 = Ponto(raio * cos(temp_aStack) * sin(temp_aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(temp_aSlice));
                Ponto ponto_6 = Ponto(raio * cos(temp_aStack) * sin(aSlice), raio * sin(temp_aStack), raio * cos(temp_aStack) * cos(aSlice));
                pontos.push_back(ponto_4);
                pontos.push_back(ponto_5);
                pontos.push_back(ponto_6);
                normais.push_back(normalizarPonto(ponto_4));
                normais.push_back(normalizarPonto(ponto_5));
                normais.push_back(normalizarPonto(ponto_6));

                texturas.push_back(temp_aSlice / (2*M_PI));
                texturas.push_back((aStack + M_PI_2) / M_PI);
                texturas.push_back(temp_aSlice / (2*M_PI));
                texturas.push_back((temp_aStack + M_PI_2) / M_PI);
                texturas.push_back(aSlice / (2*M_PI));
                texturas.push_back((temp_aStack + M_PI_2) / M_PI);
            }  
			aStack += angulo_default_stack;
			temp_aStack += angulo_default_stack;
		}
	}
	*p = pontos;
    *n = normais;
    *text = texturas;
}


void writePointsToFile(vector<Ponto> points, vector<Ponto>* normais, vector<float>* texturas, string filename) {
    ofstream arquivo(filename);
    arquivo << points.size() << "\n";

    normais ? arquivo << "true\n" : arquivo << "false\n";

    texturas ? arquivo << "true\n" : arquivo << "false\n";

    for(Ponto p : points) {
        float px = p.getX();
        float py = p.getY();
        float pz = p.getZ();

        string line = to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + "\n";
        arquivo << line;
    }

    if (normais) {
        for(Ponto n : *normais) {
            float nx = n.getX();
            float ny = n.getY();
            float nz = n.getZ();

            string line = to_string(nx) + ", " + to_string(ny) + ", " + to_string(nz) + "\n";
            arquivo << line;
        }
    }

    // If defined, write vertexes texture coordinates to file
    if (texturas) {
        vector<float> ts = *texturas;
        for (int i = 0; i < ts.size(); i+=2) {
            float tx = ts[i];
            float ty = ts[i+1];

            string line = to_string(tx) + ", " + to_string(ty) + "\n";
            arquivo << line;
        }
    }

    arquivo.close();
}

int main(int argc, char** argv){
    vector<Ponto> pontos;
    vector<Ponto> normais;
    vector<float> texturas;
    string file;

    if (strcmp(argv[1],"plane") == 0){
        pontos = plane(stof(argv[2]), stof(argv[3]));
        file = argv[4];
        file = "../3dFiles/" + file;
        writePointsToFile(pontos, nullptr, nullptr, file);
    }
    else if (strcmp(argv[1],"box") == 0){
        pontos = box(stof(argv[2]), stof(argv[3]));
        file = argv[4];
        file = "../3dFiles/" + file;
        writePointsToFile(pontos, nullptr, nullptr, file);
    }
    else if (strcmp(argv[1], "cone") == 0){
        pontos = cone(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
        file = argv[6];
        file = "../3dFiles/" + file;
        writePointsToFile(pontos, nullptr, nullptr, file);
    }
    else if (strcmp(argv[1], "sphere") == 0){
        sphere(stof(argv[2]), stof(argv[3]), stof(argv[4]),&pontos,&normais,&texturas);
        file = argv[5];
        file = "../3dFiles/" + file;
        writePointsToFile(pontos, &normais, &texturas, file);
    }else if(strcmp(argv[1],"bezier") == 0){
        file = argv[4];
        file = "../3dFiles/" + file;
        bezierFile(argv[2],atoi(argv[3]),&pontos,&normais);
        writePointsToFile(pontos, &normais, nullptr, file);
    }
    else printf("Erro no input");
    
    return 0;
}