#include <iostream>
#include <vector>
#include <string>
#include "tinyxml2/tinyxml2.h"
#include <fstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#endif
#include <math.h>
#include <sstream>

GLenum modo = GL_FILL;
std::vector<std::string> modelsList;

void drawObject(){
    for (size_t i = 0; i < modelsList.size(); i++){
             
        std::ifstream file(modelsList[i]);  

        if (file.is_open()) {
        std::string line;
        int j =0;
        while (std::getline(file, line)) {
            if(i!=0){
                std::vector<std::string> row; // create vector to hold row data
                std::stringstream ss(line); // create stringstream from line

                std::string cell;
                while (std::getline(ss, cell, ',')) { // parse each cell of row based on comma delimiter
                    row.push_back(cell);
                }
                glBegin(GL_TRIANGLES);
                    glVertex3f(std::stof(row[0]),std::stof(row[1]),std::stof(row[2]));
                glEnd();
                j=1; 
            }               
        }
        file.close(); // close the file
        }else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }
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
	gluLookAt(6.0,6.0,6.0, 
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

    drawObject();
    
    
    
	
	glutPostRedisplay();
	// End of frame
	glutSwapBuffers();
}


int main(int argc , char** argv) {

    // load the XML file
    tinyxml2::XMLDocument doc;
    doc.LoadFile("example.xml");    

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

    // get the models element
    tinyxml2::XMLElement* models = root->FirstChildElement("group")
                                        ->FirstChildElement("models");
    // declare a vector to store the model file names
    

    // iterate over all model child elements
    for (tinyxml2::XMLElement* model = models->FirstChildElement("model");
        model != nullptr;
        model = model->NextSiblingElement("model")) {  

        // extract the file attribute value of the current model element
        const char* file = model->Attribute("file");

        // add the file attribute value to the vector
        modelsList.push_back(file);
    }   

    // print the model file names stored in the vector
    std::cout << "Model files:\n";
    for (auto const& file : modelsList) {
        std::cout << "- " << file << std::endl;
    }

    // print the extracted information
    std::cout << "Window width: " << width << ", height: " << height << std::endl;
    std::cout << "Camera position: (" << x << ", " << y << ", " << z << ")" << std::endl;
    std::cout << "Camera lookAt: (" << lookAtX << ", " << lookAtY << ", " << lookAtZ << ")" << std::endl;
    std::cout << "Camera up: (" << upX << ", " << upY << ", " << upZ << ")" << std::endl;
    std::cout << "Camera projection fov: " << fov << ", near: " << near << ", far: " << far << std::endl;   
    
    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM TP2");
		
    // Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

    //  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	
    // enter GLUT's main cycle
	glutMainLoop();
	  
    return 0;
}