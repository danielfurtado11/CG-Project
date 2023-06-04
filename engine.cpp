#include <iostream>
#include <vector>
#include <string>
#include "tinyxml2/tinyxml2.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <sstream>
#include "fpsCamera.h"
#include "lights.h"
#include "model.h"
#include "ponto.h"
#include "transformations.h"

using namespace std;
using namespace tinyxml2;
#define MAX 100


GLdouble alpha_angle = M_PI / 4;
GLdouble beta_angle = M_PI / 6;
GLdouble gamma_value = 40.0;

GLenum modo = GL_FILL;
std::vector<Group> groupList;
vector<Light*> lights_vector = {};

int timebase;
float frames;
float fps;
int limite;

fpsCamera* fps_camera;

Model drawObject(string texto){
    int size = 0;
    vector<float> pontos;
    vector<float> texturas;
    vector<float> normals;
    ifstream file("3dFiles/" + texto);  
    if (file.is_open()) {

        string line;
        int j =0;

        // Skip number of points in file
        getline(file, line);

        // Check if there's normals in the file
        getline(file, line);
        bool b_normals;
        line == "true" ? b_normals = true : b_normals = false;

        // Check if there's textures in the file
        getline(file, line);
        bool b_textures;
        line == "true" ? b_textures = true : b_textures = false;

        while (std::getline(file, line)) {
                vector<string> row; // create vector to hold row data
                stringstream ss(line); // create stringstream from line
                string cell;
                pontos.push_back(stof(row[0]));
                pontos.push_back(stof(row[1]));
                pontos.push_back(stof(row[2]));
                if(b_normals){
                    normals.push_back(stof(row[0]));
                    normals.push_back(stof(row[1]));
                    normals.push_back(stof(row[2]));
                }

                if(b_textures){
                    texturas.push_back(stof(row[0]));
                    texturas.push_back(stof(row[1]));
                    texturas.push_back(stof(row[2]));
                }
                size += 1;           
        }
    file.close(); // close the file

    }else {
        cerr << "Unable to open file." << endl;
    }

	GLuint verticeCount = (GLuint) (size);
	GLuint p_vbo_ind;
	GLuint n_vbo_ind = 0;
	GLuint t_vbo_ind = 0;
    std::cout << glGenBuffers << std::endl;
	glGenBuffers(1, &p_vbo_ind);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo_ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pontos.size(), pontos.data(), GL_STATIC_DRAW);

    if (b_normals) {
		// Push normals to VBO
		glGenBuffers(1, &n_vbo_ind);
		glBindBuffer(GL_ARRAY_BUFFER, n_vbo_ind);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
	}

	if (b_textures) {
		// Push textures to VBO
		glGenBuffers(1, &t_vbo_ind);
		glBindBuffer(GL_ARRAY_BUFFER, t_vbo_ind);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texturas.size(), texturas.data(), GL_STATIC_DRAW);
	}


	return Model(p_vbo_ind, n_vbo_ind, t_vbo_ind, verticeCount);
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

        TranslacaoG* t_dt = dynamic_cast<TranslacaoG*>(t);
		if (t_dt) {
			t_dt->renderCatmullRomCurve();
			t_dt->applyTransformations();
			continue;
		}

		Rotacao* tR = dynamic_cast<Rotacao*>(t);
		if (tR) {
			glRotatef(tR->getAng(), tR->getX(), tR->getY(), tR->getZ());
			continue;
		}

        RotacaoG* t_dr = dynamic_cast<RotacaoG*>(t);
		if (t_dr) {
			t_dr->applyTransformation();
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
	

    std::vector<Model> modelsList = g.getModelsList();

    for(Model m : modelsList){

        // Set model material properties
        glMaterialfv(GL_FRONT, GL_AMBIENT, m.getAmbient());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m.getDiffuse());
        glMaterialfv(GL_FRONT, GL_SPECULAR, m.getSpecular());
        glMaterialfv(GL_FRONT, GL_EMISSION, m.getEmissive());
        glMaterialf(GL_FRONT, GL_SHININESS, m.getShininess());

        // Bind points VBO
        glBindBuffer(GL_ARRAY_BUFFER, m.getPVBOInd());
        glVertexPointer(3, GL_FLOAT, 0, 0);

        // If defined, bind normals VBO
        GLuint n_vbo_ind = m.getNVBOInd();
        if (n_vbo_ind != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, n_vbo_ind);
            glNormalPointer(GL_FLOAT, 0, 0);
        }

        // If defined, bind textures VBO
        GLuint t_vbo_ind = m.getTVBOInd();
        if (t_vbo_ind != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, t_vbo_ind);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
            glBindTexture(GL_TEXTURE_2D, m.getTextureID());
        }

        glDrawArrays(GL_TRIANGLES, 0, m.getVerticeCount());

        glBindTexture(GL_TEXTURE_2D, 0);

    }

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

    gluLookAt(fps_camera->getEyeX(), fps_camera->getEyeY(), fps_camera->getEyeZ(),
		      	fps_camera->getCenterX(), fps_camera->getCenterY(), fps_camera->getCenterZ(),
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

	char s[10];
	frames++;
	float time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frames*1000.0/(time-timebase);
		timebase = time;
		frames = 0;
	}	
	sprintf(s, "%f", fps);
	glutSetWindowTitle(s);

    for (Light* l : lights_vector) {
		l->apply();
	}

    for(Group g : groupList){
        drawGroup(g);
    }

  
	glutPostRedisplay();
	// End of frame
	glutSwapBuffers();
}

void myKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {

        case 'a':
			 fps_camera->reactcamerafps(key);
             break;
        case 'd':
			fps_camera->reactcamerafps(key); 
			break;
		case 'w':
			fps_camera->reactcamerafps(key);
			break;
		case 's':
			fps_camera->reactcamerafps(key) ;
			break;
        case 'q':
            fps_camera->reactcamerafps(key) ;
			break;
        case 'e':
            fps_camera->reactcamerafps(key) ;
			break;
        case 'c':
            fps_camera->carregar_camera("fpscamera.cfg");
            break;
        case 'g':
            fps_camera->guarda_camera("fpscamera.cfg");
            break;
        case 'b': modo = GL_POINT; break;
        case 'n': modo = GL_LINE; break;
        case 'm': modo = GL_FILL; break;
    }
}

// Function to process mouse motion
void processMouseMotion(int xx, int yy) {
		fps_camera->processar_movimento_rato(xx, yy);
		glutPostRedisplay();
}

// Function to process mouse buttons
void processMouseButtons(int button, int state, int xx, int yy) {
		fps_camera->processar_botoes_rato(button, state, xx, yy);
		glutPostRedisplay();
}



Light* parseXMLLightElement (XMLElement* light_element, int light_ind) {
	Light* new_light = nullptr;

	// Get light type: POINT, DIRECTIONAL or SPOT
	const XMLAttribute* type_attribute = light_element->FindAttribute("type");
	const char* light_type;
	type_attribute ? light_type = type_attribute->Value() : light_type = "";

	// Get ambient attributes
	GLfloat* ambient = parseAmbientAttributes(light_element, 0.0);

	// Get diffuse attributes
	GLfloat* diffuse = parseDiffuseAttributes(light_element, 1.0);

	// Get specular attributes
	GLfloat* specular = parseSpecularAttributes(light_element, 1.0);

	if (strcmp(light_type, "POINT") == 0) {
		float posX = parseFloatFromElementAttribute(light_element, "posX", 0.0);
		float posY = parseFloatFromElementAttribute(light_element, "posY", 0.0);
		float posZ = parseFloatFromElementAttribute(light_element, "posZ", 0.0);

		new_light = new LightPoint(light_ind, new Ponto(posX, posY, posZ), ambient, diffuse, specular);
	}
	else if (strcmp(light_type, "DIRECTIONAL") == 0) {
		float dirX = parseFloatFromElementAttribute(light_element, "dirX", 0.0);
		float dirY = parseFloatFromElementAttribute(light_element, "dirY", 0.0);
		float dirZ = parseFloatFromElementAttribute(light_element, "dirZ", 1.0);

		new_light = new LightDirectional(light_ind, new Ponto(dirX, dirY, dirZ), ambient, diffuse, specular);
	}
	else if (strcmp(light_type, "SPOT") == 0) {
		float posX = parseFloatFromElementAttribute(light_element, "posX", 0.0);
		float posY = parseFloatFromElementAttribute(light_element, "posY", 0.0);
		float posZ = parseFloatFromElementAttribute(light_element, "posZ", 0.0);

		float dirX = parseFloatFromElementAttribute(light_element, "dirX", 0.0);
		float dirY = parseFloatFromElementAttribute(light_element, "dirY", 0.0);
		float dirZ = parseFloatFromElementAttribute(light_element, "dirZ", -1.0);

		float cutoff = parseFloatFromElementAttribute(light_element, "cutoff", 180.0);

		new_light = new LightSpot(light_ind, new Ponto(posX, posY, posZ), new Ponto(dirX, dirY, dirZ), cutoff, ambient, diffuse, specular);
	}

	return new_light;
}

// Function to parse ambient attributes in a light or model element
GLfloat* parseAmbientAttributes(XMLElement* element, GLfloat default_value) {
	const XMLAttribute* aux_attribute;
	GLfloat* ambient = new GLfloat[4];
	
	ambient[0] = parseFloatFromElementAttribute(element, "ambiR", default_value);
	ambient[1] = parseFloatFromElementAttribute(element, "ambiG", default_value);
	ambient[2] = parseFloatFromElementAttribute(element, "ambiB", default_value);
	ambient[3] = parseFloatFromElementAttribute(element, "ambiA", 1.0);

	return ambient;
}

// Function to parse diffuse attributes in a light or model element
GLfloat* parseDiffuseAttributes(XMLElement* element, GLfloat default_value) {
	const XMLAttribute* aux_attribute;
	GLfloat* diffuse = new GLfloat[4];
	
	diffuse[0] = parseFloatFromElementAttribute(element, "diffR", default_value);
	diffuse[1] = parseFloatFromElementAttribute(element, "diffG", default_value);
	diffuse[2] = parseFloatFromElementAttribute(element, "diffB", default_value);
	diffuse[3] = parseFloatFromElementAttribute(element, "diffA", 1.0);

	return diffuse;
}

// Function to parse specular attributes in a light or model element
GLfloat* parseSpecularAttributes(XMLElement* element, GLfloat default_value) {
	const XMLAttribute* aux_attribute;
	GLfloat* specular = new GLfloat[4];
	
	specular[0] = parseFloatFromElementAttribute(element, "specR", default_value);
	specular[1] = parseFloatFromElementAttribute(element, "specG", default_value);
	specular[2] = parseFloatFromElementAttribute(element, "specB", default_value);
	specular[3] = parseFloatFromElementAttribute(element, "specA", 1.0);

	return specular;
}

// Function to parse emissive attributes in a model element
GLfloat* parseEmissiveAttributes(XMLElement* element, GLfloat default_value) {
	const XMLAttribute* aux_attribute;
	GLfloat* emissive = new GLfloat[4];
	
	emissive[0] = parseFloatFromElementAttribute(element, "emisR", default_value);
	emissive[1] = parseFloatFromElementAttribute(element, "emisG", default_value);
	emissive[2] = parseFloatFromElementAttribute(element, "emisB", default_value);
	emissive[3] = parseFloatFromElementAttribute(element, "emisA", 1.0);

	return emissive;
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
                const char* timeValue = childDois->Attribute("time");

                if (timeValue != nullptr) {
		            vector<Ponto> points = {};

		            string closed = childDois->Attribute("closed");

		            // The points describe a closed line, so we need to duplicate some of them
		            if (closed.compare("true") == 0) {
		            	vector<Ponto> temp_points;

		            	tinyxml2::XMLElement* point_element = childDois->FirstChildElement("point");

		            	while (point_element) {
		            		float x_value = atof(point_element->FindAttribute("x")->Value());
		            		float y_value = atof(point_element->FindAttribute("y")->Value());
		            		float z_value = atof(point_element->FindAttribute("z")->Value());

		            		temp_points.push_back( Ponto(x_value, y_value, z_value) );

		            		point_element = point_element->NextSiblingElement("point");
		            	}

		            	// Duplicate some points so we get a closed curve
		            	points.push_back(temp_points.back());
		            	for (int i = 0; i < temp_points.size(); i++)
		            		points.push_back(temp_points[i]);
		            	points.push_back(temp_points[0]);
		            	points.push_back(temp_points[1]);
		            }

		            // The points can describe a closed line, but none are duplicated and all are interpretated
		            // as part of a Catmull-Rom cubic curve
		            else {
		            	tinyxml2::XMLElement* point_element = childDois->FirstChildElement("point");

		            	while (point_element) {
		            		float x_value = atof(point_element->FindAttribute("x")->Value());
		            		float y_value = atof(point_element->FindAttribute("y")->Value());
		            		float z_value = atof(point_element->FindAttribute("z")->Value());

		            		points.push_back( Ponto(x_value, y_value, z_value) );

		            		point_element = point_element->NextSiblingElement("point");
		            	}
		            }


		            group.addTranslacaoG(std::strtof(timeValue, nullptr), points);
	            }

                else {
                    double x = childDois->DoubleAttribute("x");
                    double y = childDois->DoubleAttribute("y");
                    double z = childDois->DoubleAttribute("z");
                    group.addTranslacao(x,y,z);
                }

                
            }
            else if (strcmp(elemento, "rotate") == 0) {
                double angle = childDois->DoubleAttribute("angle");
                double x = childDois->DoubleAttribute("x");
                double y = childDois->DoubleAttribute("y");
                double z = childDois->DoubleAttribute("z");
	            double time = childDois->DoubleAttribute("time");

	            // Test if we have a dynamic rotation
	            if (time) {
                    std::cout << "TEMPO: " << time;
	            	group.addRotacaoG(time, x, y, z);
	            }

	            // Static rotation
	            else {
	            	// Get angle attribute
	            	const char* angle_attribute = childDois->Attribute("angle");
	            	float angle_rot;
	            	angle_attribute ? angle_rot = std::strtof(angle_attribute, nullptr) : angle_rot = 0;

	            	group.addRotacao(angle_rot, x, y, z);
	            }
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
            Model m = drawObject(file);

            // Get diffuse attributes
			GLfloat* diffuse = parseDiffuseAttributes(model_element, 0.8);
			model.setDiffuse(diffuse);

			// Get specular attributes
			GLfloat* specular = parseSpecularAttributes(model_element, 0.0);
			model.setSpecular(specular);

			// Get emissive attributes
			GLfloat* emissive = parseEmissiveAttributes(model_element, 0.0);
			model.setEmissive(emissive);

			// Get ambient attributes
			GLfloat* ambient = parseAmbientAttributes(model_element, 0.2);
			model.setAmbient(ambient);

			// Get texture attribute
			const XMLAttribute* texture_attribute = model_element->FindAttribute("texture");
			string texture_file;
			texture_attribute ? texture_file = texture_attribute->Value() : texture_file = "";
			
			model.loadTexture(BIN_IMAGE_DIR + texture_file);
            group.addModels(m);
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

    // Trying to get lights element
	tinyxml2::XMLElement* lights_element = root->FirstChildElement("lights");
	if (lights_element) {
		int light_ind = 0;
		
		tinyxml2::XMLElement* light_element = lights_element->FirstChildElement("light");
		while (light_element) {
			Light* l = parseXMLLightElement(light_element, light_ind);
			if (l != nullptr)
				lights_vector->push_back(l);

			light_element = light_element->NextSiblingElement("light");
			light_ind++;
		}
	}

    // Ler o group
    tinyxml2::XMLElement* grupos = root->FirstChildElement("group");
	while (grupos) {
		Group g = loadGroupXML(grupos);
		groupList.push_back(g);

		grupos = grupos->NextSiblingElement("group");
	}
}



int main(int argc , char** argv) {

    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM TP2");
    glEnableClientState(GL_VERTEX_ARRAY);

    // Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    glutKeyboardFunc(myKeyboardFunc);
    glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutIdleFunc(renderScene);


    #ifndef __APPLE__
	glewInit();
    #endif

    loadXML(argv[1]);
    timebase = glutGet(GLUT_ELAPSED_TIME);


    //  OpenGL settings
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	// Enable lights parsed in XML file
	for (int i = 0; i < lights_vector.size(); i++)
		glEnable(GL_LIGHT0 + (GLenum) i);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fps_camera = new fpsCamera(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), "fpscamera.cfg");
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2);
	
    // enter GLUT's main cycle
	glutMainLoop();
	  
    return 0;
}