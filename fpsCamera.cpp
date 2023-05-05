#include <stdlib.h>

#include <GL/glut.h>


#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "fpsCamera.h"

using namespace std;

fpsCamera::fpsCamera(int width, int height, string cfg_file_str) {
	// loads camera settings from cfg file
	carregar_camera(cfg_file_str);

	// sets mouse starting point
	startX = width / 2;
	startY = height / 2;

	// disables mouse tracking
	tracking[0] = 0;
	tracking[1] = 0;
}

GLdouble fpsCamera::getCenterX() {
	return eyeX + sin(alpha) * sin(beta);
}

GLdouble fpsCamera::getCenterY() {
	return eyeY + cos(beta);
}

GLdouble fpsCamera::getCenterZ() {
	return eyeZ + cos(alpha) * sin(beta);
}


void fpsCamera::reactcamerafps(unsigned char c) {
    switch (c) {
        case 'a':
			eyeX -= sin(alpha - M_PI/2) * speed;
        	eyeZ -= cos(alpha - M_PI/2) * speed;
			break;
        case 'd':
			eyeX -= sin(alpha + M_PI/2) * speed;
        	eyeZ -= cos(alpha + M_PI/2) * speed;
			break;
		case 'w':
			eyeX += sin(alpha) * speed;
			eyeZ += cos(alpha) * speed;
			break;
		case 's':
			eyeX -= sin(alpha) * speed;
			eyeZ -= cos(alpha) * speed;
			break;
		case 'q':
			eyeY += 0.3 * speed;
			break;
		case 'e':
			eyeY -= 0.3 * speed;
			break;
    }

}

void fpsCamera::processar_movimento_rato(int xx, int yy) {
    int deltaX = xx - startX;
    startX = xx;

    alpha -= deltaX * sensitivity;

    int deltaY = yy - startY;
    startY = yy;

    beta += deltaY * sensitivity;

    if (beta < M_PI/32) beta = M_PI/32;
    else if (beta > M_PI - M_PI/32) beta = M_PI - M_PI/32;
}

void fpsCamera::processar_botoes_rato(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
        startX = xx;
        startY = yy;
    }
}


// Carrega o cfg da camera
void fpsCamera::carregar_camera(string file_str) {
    string linha;
    ifstream ficheiro(file_str);

    if (ficheiro.is_open()) {
        while (getline(ficheiro, linha)) {
            size_t pos = linha.find(":");
            if (pos != string::npos) {
                string token = linha.substr(0, pos);
                string value = linha.substr(pos + 1);

                if (token == "alpha")
                    this->alpha = stod(value);
                else if (token == "beta")
                    this->beta = stod(value);
                else if (token == "eyeX")
                    this->eyeX = stod(value);
                else if (token == "eyeY")
                    this->eyeY = stod(value);
                else if (token == "eyeZ")
                    this->eyeZ = stod(value);
                else if (token == "sensitivity")
                    this->sensitivity = stof(value);
                else if (token == "speed")
                    this->speed = stof(value);
            }
        }

        ficheiro.close();
    } else {
        cout << "Não foi possível abrir o ficheiro de cfg da camera" << endl;
    }
}

// Vai guardar o estado da camera no cfg
void fpsCamera::guarda_camera(string file_str) {
    ofstream ficheiro(file_str);

    if (ficheiro.is_open()) {
        ficheiro << "alpha:" << this->alpha << "\n";
        ficheiro << "beta:" << this->beta << "\n";
        ficheiro << "eyeX:" << this->eyeX << "\n";
        ficheiro << "eyeY:" << this->eyeY << "\n";
        ficheiro << "eyeZ:" << this->eyeZ << "\n";
        ficheiro << "sensitivity:" << this->sensitivity << "\n";
        ficheiro << "speed:" << this->speed << "\n";

        ficheiro.close();
    } else {
        cout << "Não foi possível abrir o ficheiro para guardar a cfg" << "\n";
    }
}