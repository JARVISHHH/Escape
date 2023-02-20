#include "screen.h"

Screen::Screen() :camera(std::make_shared<Camera>()) {
	
}

Screen::~Screen() {

}

void Screen::init()
{
}

void Screen::update(double seconds) {
	
}

void Screen::draw() {
	
}

void Screen::keyEvent(int key, int action) {

}

void Screen::mousePosEvent(double xpos, double ypos) {

}

void Screen::mouseButtonEvent(int button, int action) {

}

void Screen::scrollEvent(double distance) {

}

void Screen::framebufferResizeEvent(int width, int height) {
	Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void Screen::windowResizeEvent(int width, int height) {
	Global::graphics.setWindowSize(glm::ivec2(width, height));
	camera->resize(width, height);
}