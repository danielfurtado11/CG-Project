#include <iostream>
#include <vector>
#include <string>
#include "tinyxml2/tinyxml2.h"
#include <fstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <sstream>
#include "transformations.h"

using namespace std;



GLdouble alpha_angle = M_PI / 4;
GLdouble beta_angle = M_PI / 6;
GLdouble gamma_value = 40.0;

GLenum modo = GL_FILL;
std::vector<Group> groupList;

void drawObject(std::vector<std::string> modelsList){
    for (size_t i = 0; i < modelsList.size(); i++){
        ifstream file("3dFiles/" + modelsList[i]) ;  

        if (file.is_open()) {
            
            string line;
            int j =0;
            int jx = 0;
            vector<string> jacinto;

            while (std::getline(file, line)) {

                if(j!=0){
                    vector<string> row; // create vector to hold row data
                    stringstream ss(line); // create stringstream from line


                    string cell;
                    while (getline(ss, cell, ',')) { // parse each cell of row based on comma delimiter
                        row.push_back(cell);
                    }

                    if (jx == 0){
                        glBegin(GL_TRIANGLES);
                    }
                    jx += 1;
                    glVertex3f(stof(row[0]),stof(row[1]),stof(row[2]));
                    if (jx == 3){
                        glEnd();
                        jx = 0;
                    }
                }    
                j=1;            
            }
        file.close(); // close the file
        }else {
            cerr << "Unable to open file." << endl;
        }
    }
}

void drawGroup(Group g){

	// Drawing objects in this group

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


    vector<Transformation*> tV = g.getTransformations();
	for (Transformation* t : tV) {
		Translacao* tT = dynamic_cast<Translacao*>(t);
		if (tT) {
			glTranslatef(tT->getX(), tT->getY(), tT->getZ());
			continue;
		}

		Rotacao* tR = dynamic_cast<Rotacao*>(t);
		if (tR) {
			glRotatef(tR->getAng(), tR->getX(), tR->getY(), tR->getZ());
			continue;
		}

		Escala* tS = dynamic_cast<Escala*>(t);
		if (tS) {
			glScalef(tS->getX(), tS->getY(), tS->getZ());
			continue;
		}
	}

    Color* cl = g.getColor();
    if (cl != nullptr){
        glColor3f(cl->getR(), cl->getG(), cl->getB());
    }
	

    std::vector<std::string> modelsList = g.getModelsList();
	drawObject(modelsList);


    // Drawing groups in this group
    vector<Group> groups = g.getGroups();
	for (Group group : groups) {
		drawGroup(group);
	}
    glPopMatrix();

}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


// o translate tem de estar antes do teclado e depois do lookat, com variáveis globais
// o angulo é em graus, e o eixo de rotação é o vetor que define o espeto de um frango

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity(); // Destroi todas as entidades feitas, é mesmo necessário acontecer
	gluLookAt(sin(alpha_angle)*cos(beta_angle)*gamma_value, sin(beta_angle)*gamma_value, cos(alpha_angle)*cos(beta_angle)*gamma_value,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, modo);

    // put axis drawing in here
	glBegin(GL_LINES);
	// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
	// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 50.0f);
	glEnd();

    for(Group g : groupList){
        drawGroup(g);
    }
  
	glutPostRedisplay();
	// End of frame
	glutSwapBuffers();
}

void myKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': alpha_angle -= M_PI / 32; break;
        case 'd': alpha_angle += M_PI / 32; break;
        case 'w':
            beta_angle += M_PI / 32;
            if (beta_angle > 1.5) beta_angle = 1.5;
            break;
        case 's':
            beta_angle -= M_PI / 32;
            if (beta_angle < -1.5) beta_angle = -1.5;
            break;
        case 'q': gamma_value -= 0.5; break;
        case 'e': gamma_value += 0.5; break;
        case 'b': modo = GL_POINT; break;
        case 'n': modo = GL_LINE; break;
        case 'm': modo = GL_FILL; break;
    }
}


Group loadGroupXML(tinyxml2::XMLElement* child){

    Group group = Group();
    
    tinyxml2::XMLElement* elemento = child->FirstChildElement("transform");
    if (elemento != nullptr) {
        std::cout << "Entrei no Transform\n";
        for (tinyxml2::XMLElement* childDois = elemento->FirstChildElement(); 
        childDois != nullptr; 
        childDois = childDois->NextSiblingElement()) {
            const char* elemento = childDois->Value();
            std::cout << "ElementoT: " << elemento << "\n";
            
            if(strcmp(elemento, "translate") == 0){
                double x = childDois->DoubleAttribute("x");
                double y = childDois->DoubleAttribute("y");
                double z = childDois->DoubleAttribute("z");
                group.addTranslacao(x,y,z);
            }
            else if (strcmp(elemento, "rotate") == 0) {
                double angle = childDois->DoubleAttribute("angle");
                double x = childDois->DoubleAttribute("x");
                double y = childDois->DoubleAttribute("y");
                double z = childDois->DoubleAttribute("z");
                group.addRotacao(angle,x,y,z);
            }
            else if (strcmp(elemento, "scale") == 0) {
                double x = childDois->DoubleAttribute("x");
                double y = childDois->DoubleAttribute("y");
                double z = childDois->DoubleAttribute("z");
                group.addEscala(x,y,z);
            }
            else if (strcmp(elemento, "color") == 0) {
                float r = childDois->FloatAttribute("r");
                float g = childDois->FloatAttribute("g"); 
                float b = childDois->FloatAttribute("b");
                group.setColor(r,g,b);
            }
            else {
                std::cout << "Erro no Transform\n";
            }
        }
        
    } 
    
    tinyxml2::XMLElement* elemento2 = child->FirstChildElement("models");
    if (elemento2 != nullptr) {
        for (tinyxml2::XMLElement* model = elemento2->FirstChildElement("model");
            model != nullptr;
            model = model->NextSiblingElement("model")) {  
            
            // extract the file attribute value of the current model element
            const char* file = model->Attribute("file");
            std::cout << "Model: " << file << "\n";
            // add the file attribute value to the vector
            group.addModels(file);
        }     
        
    }

    tinyxml2::XMLElement* elemento3 = child->FirstChildElement("group");
    while (elemento3 != nullptr) {
        std::cout << "OII\n";
        Group g = loadGroupXML(elemento3);
		group.addGroup(g);
		elemento3 = elemento3->NextSiblingElement("group");
    }

    return group;
}


void loadXML(char* ficheiro){

// load the XML file
    tinyxml2::XMLDocument doc;
    doc.LoadFile(ficheiro);    

    // get the root element
    tinyxml2::XMLElement* root = doc.FirstChildElement("world");   

    // get the window element and extract the width and height attributes
    tinyxml2::XMLElement* window = root->FirstChildElement("window");
    int width = window->IntAttribute("width");
    int height = window->IntAttribute("height");

    // get the camera element and extract its child elements and attributes
    tinyxml2::XMLElement* camera = root->FirstChildElement("camera");
    tinyxml2::XMLElement* position = camera->FirstChildElement("position");
    tinyxml2::XMLElement* lookAt = camera->FirstChildElement("lookAt");
    tinyxml2::XMLElement* up = camera->FirstChildElement("up");
    tinyxml2::XMLElement* projection = camera->FirstChildElement("projection");

    double x = position->DoubleAttribute("x");
    double y = position->DoubleAttribute("y");
    double z = position->DoubleAttribute("z");
    double lookAtX = lookAt->DoubleAttribute("x");
    double lookAtY = lookAt->DoubleAttribute("y");
    double lookAtZ = lookAt->DoubleAttribute("z");
    double upX = up->DoubleAttribute("x");
    double upY = up->DoubleAttribute("y");
    double upZ = up->DoubleAttribute("z");
    double fov = projection->DoubleAttribute("fov");
    double near = projection->DoubleAttribute("near");
    double far = projection->DoubleAttribute("far");    

    // Ler o group
    tinyxml2::XMLElement* grupos = root->FirstChildElement("group");
	while (grupos) {
		Group g = loadGroupXML(grupos);
		groupList.push_back(g);

		grupos = grupos->NextSiblingElement("group");
	}
}



int main(int argc , char** argv) {

    loadXML(argv[1]);

    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM TP2");
		
    // Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    glutKeyboardFunc(myKeyboardFunc);

    //  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	
    // enter GLUT's main cycle
	glutMainLoop();
	  
    return 0;
}