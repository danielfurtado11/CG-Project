#ifndef MODEL_H
#define MODEL_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

class Model {
    private:
        GLuint vbo_ind;
        GLsizei verticeCount;
    public:
        Model() {
            this->vbo_ind = 0;
            this->verticeCount = 0;
        };
        Model(GLuint vbo_ind, GLsizei verticeCount) {
            this->vbo_ind = vbo_ind;
            this->verticeCount = verticeCount;
        };
        
        GLuint getVBOInd() {return this->vbo_ind;};
        GLsizei getVerticeCount() {return this->verticeCount;};
        void setVerticeCount(GLsizei size) {this->verticeCount = size;};
        void setVBOInd(GLuint x) {this->vbo_ind = x;};
};

#endif //MODEL_H