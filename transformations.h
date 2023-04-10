#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Transformation {
    public:
        virtual void smt(){};
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
    public:
        Group();
        void addTranslacao(float x, float y, float z);
        void addRotacao(float ang, float x, float y, float z);
        void addEscala(float x, float y, float z);
        vector<Transformation*> getTransformations();

        void addModels(string model);
        vector<string> getModelsList();

        void addGroup(Group group);
        vector<Group> getGroups();
};

#endif