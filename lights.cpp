#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ponto.h"
#include "lights.h"

void LightPoint::apply() {

    int index = this->getIndex();

    // Set light properties
    glLightfv(GL_LIGHT0+index, GL_AMBIENT, this->getAmbient());
    glLightfv(GL_LIGHT0+index, GL_DIFFUSE, this->getDiffuse());
    glLightfv(GL_LIGHT0+index, GL_SPECULAR, this->getSpecular());

    // Set point light properties
    GLfloat pos[4] = {this->pos->getX(), this->pos->getY(), this->pos->getZ(), 1.0};
    glLightfv(GL_LIGHT0+index, GL_POSITION, pos);
}

void LightDirectional::apply() {

    int index = this->getIndex();

    // Set light properties
    glLightfv(GL_LIGHT0+index, GL_AMBIENT, this->getAmbient());
    glLightfv(GL_LIGHT0+index, GL_DIFFUSE, this->getDiffuse());
    glLightfv(GL_LIGHT0+index, GL_SPECULAR, this->getSpecular());

    // Set directional light properties
    GLfloat dir[4] = {this->dir->getX(), this->dir->getY(), this->dir->getZ(), 0.0};
    glLightfv(GL_LIGHT0+index, GL_POSITION, dir);
}

void LightSpot::apply() {

    int index = this->getIndex();

    // Set light properties
    glLightfv(GL_LIGHT0+index, GL_AMBIENT, this->getAmbient());
    glLightfv(GL_LIGHT0+index, GL_DIFFUSE, this->getDiffuse());
    glLightfv(GL_LIGHT0+index, GL_SPECULAR, this->getSpecular());

    // Set spot light properties
    GLfloat pos[4] = {this->pos->getX(), this->pos->getY(), this->pos->getZ(), 1.0};
    glLightfv(GL_LIGHT0+index, GL_POSITION, pos);

    GLfloat dir[3] = {this->dir->getX(), this->dir->getY(), this->dir->getZ()};
    glLightfv(GL_LIGHT0+index, GL_SPOT_DIRECTION, dir);

    glLightfv(GL_LIGHT0+index, GL_SPOT_CUTOFF, &this->cutoff);

}