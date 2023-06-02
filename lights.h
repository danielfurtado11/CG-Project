#ifndef LIGHTS_H
#define LIGHTS_H

class Light {
    private:
        int index;
        GLfloat* ambient;
        GLfloat* diffuse;
        GLfloat* specular;
    public:
        Light() {
            this->index = -1;
            this->ambient = nullptr;
            this->diffuse = nullptr;
            this->specular = nullptr;
        };
        Light(int index, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular) {
            this->index = index;
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
        };

        virtual void apply() = 0;

        int getIndex() {return this->index;};
        GLfloat* getAmbient() {return this->ambient;};
        GLfloat* getDiffuse() {return this->diffuse;};
        GLfloat* getSpecular() {return this->specular;};

        void setIndex(int index) {this->index = index;};
        void setAmbient(GLfloat* ambient) {this->ambient = ambient;};
        void setSpecular(GLfloat* specular) {this->specular = specular;};
        void setDiffuse(GLfloat* diffuse) {this->diffuse = diffuse;};
};

class LightPoint : public Light {
    private:
        Ponto* pos;
    public:
        LightPoint() : Light() {
            this->pos = nullptr;
        };
        LightPoint(int index, Ponto* pos, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular)
            : Light(index, ambient, diffuse, specular)
        {
            this->pos = pos;
        };

        void apply();

        Ponto* getPos() {return this->pos;};
        void setPos(Ponto* pos) {this->pos = pos;};    
};

class LightDirectional : public Light {
    private:
        Ponto* dir;
    public:
        LightDirectional() : Light() {
            this->dir = nullptr;
        };
        LightDirectional(int index, Ponto* dir, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular)
            : Light(index, ambient, diffuse, specular)
        {
            this->dir = dir;
        };

        void apply();

        Ponto* getDir() {return this->dir;};
        void setDir(Ponto* dir) {this->dir = dir;};    
};

class LightSpot : public Light {
    private:
        Ponto* pos;
        Ponto* dir;
        float cutoff;
    public:
        LightSpot() : Light() {
            this->pos = nullptr;
            this->dir = nullptr;
            this->cutoff = 180;
        };
        LightSpot(int index, Ponto* pos, Ponto* dir, float cutoff, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular)
            : Light(index, ambient, diffuse, specular)
        {
            this->pos = pos;
            this->dir = dir;
            this->cutoff = cutoff;
        };

        void apply();

        Ponto* getPos() {return this->pos;};
        void setPos(Ponto* pos) {this->pos = pos;};    

        Ponto* getDir() {return this->dir;};
        void setDir(Ponto* dir) {this->dir = dir;};    

        float getCutOff() {return this->cutoff;};
        void setCutOff(float cutoff) {this->cutoff = cutoff;};    
};

#endif //LIGHTS_H