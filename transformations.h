#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <GL/glut.h>

#include "ponto.h"
#include "Matrix.tpp"

using namespace std;

class Transformation {
    public:
        virtual void smt(){};
};

class Color {
    private:
        float r, g, b;
    public:
        float getR() {return this->r;};
        float getG() {return this->g;};
        float getB() {return this->b;};
        Color();
        Color(float r, float g, float b) {
            this->r = r;
            this->g = g;
            this->b = b;
        };
};

class Translacao : public Transformation {
    private:
        float x, y, z;
    public:
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getZ() {return this->z;};
        Translacao();
        Translacao(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
};

class TranslacaoG : public Transformation {
    private:
        float total_time;  // time to run the whole curve
        float segment_time;  // time to run each segment
        float timebase;  // last time measured
        float elapsed_time;  // time elapsed since beginning of the curve
        vector<Ponto> points;
        vector<Ponto> render_points;

        void generateRenderPoints();
    public:
        void applyTransformations();
        void renderCatmullRomCurve();

        TranslacaoG();
        TranslacaoG(float total_time, vector<Ponto> points);    
};

class Rotacao : public Transformation {
    private:
        float ang, x, y, z;
    public:
        float getAng() {return this->ang;};
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getZ() {return this->z;};
        Rotacao();
        Rotacao(float ang, float x, float y, float z) {
            this->ang = ang;
            this->x = x;
            this->y = y;
            this->z= z;
        };
};

class RotacaoG : public Transformation {
    private:
        float total_time;  // time to perform 360 degrees rotation
        float timebase;  // time at the start of the rotation
        float axisX, axisY, axisZ;  // axis of rotation
    public:
        void applyTransformation();

        RotacaoG();
        RotacaoG(float total_time, float axisX, float axisY, float axisZ);    
};

class Escala : public Transformation {
    private:
        float x, y, z;
    public:
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getZ() {return this->z;};
        Escala();
        Escala(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
};

class Group {
    private:
        vector<Transformation*> transformations;
        vector<string> modelsList;
        vector<Group> groups;
        Color* color;
    public:
        Group();
        void addTranslacao(float x, float y, float z);
        void addTranslacaoG(float time, vector<Ponto> points);
        void addRotacao(float ang, float x, float y, float z);
        void addRotacaoG(float time, float axisX, float axisY, float axisZ);
        void addEscala(float x, float y, float z);
        vector<Transformation*> getTransformations();

        void addModels(string model);
        vector<string> getModelsList();

        void addGroup(Group group);
        vector<Group> getGroups();

        void setColor(float r, float g, float b);
        Color* getColor();
};

#endif