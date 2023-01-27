#include "core.h"

Core::Core(){
    m_cone = Global::graphics.getShape("cone");
    camera = std::make_shared<Camera>();

    shape = Global::graphics.getShape("cube");
    modelTransform = std::make_shared<ModelTransform>();
    modelTransform->rotate(30.0 / 180 * 3.14, glm::vec3(1, 0, 0));
    modelTransform->rotate(30.0 / 180 * 3.14, glm::vec3(0, 0, 1));
    modelTransform->translate(glm::vec3(0, 0, 3));

    Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
}

Core::~Core(){

}

void Core::update(){

}

void Core::draw(){
    Global::graphics.setClearColor(glm::vec3(0.0, 1.0, 1.0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Global::graphics.bindShader("phong");
    Global::graphics.setGlobalData(glm::vec3(0.5f));
    Global::graphics.setCameraData(camera);
    Global::graphics.drawShape(shape, modelTransform, Global::graphics.getMaterial("grass"));

    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Welcome to CS1950U!", glm::ivec2(0, 200), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.f, 0.1f, glm::vec3(1, 0, 1));
}

void Core::keyEvent(int key, int action){
    
}

void Core::mousePosEvent(double xpos, double ypos){

}

void Core::mouseButtonEvent(int button, int action){

}

void Core::scrollEvent(double distance){
    
}

void Core::framebufferResizeEvent(int width, int height){
    Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void Core::windowResizeEvent(int width, int height){
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}