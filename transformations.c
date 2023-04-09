#include <string>
#include <sstream>
#include <vector>

#include "group.h"

using namespace std;

Group::Group(){
    transformations = {};
    modelsList = {};
    groups = {};
}

Escala::Escala(){
    x=0;
    y=0;
    z=0;    
}

Rotacao::Rotacao(){
    ang=0;
    x=0;
    y=0;
    z=0;    
}

Translacao::Translacao(){
    x=0;
    y=0;
    z=0;    
}

Group::addTranslacao(float x, float y, float z){
    Translate* t = new Translacao(x,y,z)
    this->transformations.push_back(t);
}

Group::addEscala(float x, float y, float z){
    Escala* e = new Escala(x,y,z)
    this->transformations.push_back(e);
}

Group::addRotacao(float ang, float x, float y, float z){
    Rotacao* a = new Rotacao(ang, x,y,z)
    this->transformations.push_back(a);
}

vector<Transformation*> Group::getTransformations() {
    return this->transformations;
}

Group::addModels(string m){
    this->modelsList.push_back(m);
}

vector<string> Group::getModelsList() {
    return this->modelsList;
}

Group::addGroup(Group g){
    this->groups.push_back(g);
}

vector<Group> Group::getGroups() {
    return this->groups;
}