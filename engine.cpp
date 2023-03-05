#include <iostream>
#include "tinyxml2.h"



int main() {
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

    // get the models element and iterate over all its model child elements
    tinyxml2::XMLElement* models = root->FirstChildElement("group")->FirstChildElement("models");
    for (tinyxml2::XMLElement* model = models->FirstChildElement("model");
         model != nullptr;
         model = model->NextSiblingElement("model")) {  

            // extract the file attribute value of the current model element
            const char* file = model->Attribute("file"); 

            // print the file attribute value
            std::cout << "Model file: " << file << std::endl;
    }   

    // print the extracted information
    std::cout << "Window width: " << width << ", height: " << height << std::endl;
    std::cout << "Camera position: (" << x << ", " << y << ", " << z << ")" << std::endl;
    std::cout << "Camera lookAt: (" << lookAtX << ", " << lookAtY << ", " << lookAtZ << ")" << std::endl;
    std::cout << "Camera up: (" << upX << ", " << upY << ", " << upZ << ")" << std::endl;
    std::cout << "Camera projection fov: " << fov << ", near: " << near << ", far: " << far << std::endl;   
    
    return 0;
}