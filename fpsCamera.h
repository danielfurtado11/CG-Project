#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include <stdlib.h>
#include <GL/glut.h>

#include <string>

using namespace std;

class fpsCamera {
    private:
        GLdouble alpha;
        GLdouble beta;
        float sensitivity;

        GLdouble eyeX;
        GLdouble eyeY;
        GLdouble eyeZ;
        float speed;

        int startX, startY;
        int tracking[2];
    public:
        fpsCamera(int width, int height, string cfg_file_str);
        GLdouble getEyeX() {return eyeX;};
        GLdouble getEyeY() {return eyeY;};
        GLdouble getEyeZ() {return eyeZ;};
        GLdouble getCenterX();
        GLdouble getCenterY();
        GLdouble getCenterZ();

        void reactcamerafps(unsigned char c);
        void processar_movimento_rato(int xx, int yy);
        void processar_botoes_rato(int button, int state, int xx, int yy);


        // Carregar e Guardar
        void carregar_camera(string file_str);
        void guarda_camera(string file_str);
};

#endif //FPS_CAMERA_H