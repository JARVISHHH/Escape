#include "core.h"

Core::Core(){

}

Core::~Core() {

}

void Core::setApp(Application* app) {
	this->app = app;
}

void Core::update(double seconds) {
	if (app != nullptr) app->update(seconds);
}

void Core::draw() {
	if (app != nullptr) app->draw();
}

void Core::keyEvent(int key, int action) {
	if (app != nullptr) app->keyEvent(key, action);
}

void Core::mousePosEvent(double xpos, double ypos) {
	if (app != nullptr) app->mousePosEvent(xpos, ypos);
}

void Core::mouseButtonEvent(int button, int action) {
	if (app != nullptr) app->mouseButtonEvent(button, action);
}

void Core::scrollEvent(double distance) {
	if (app != nullptr) app->scrollEvent(distance);
}

void Core::framebufferResizeEvent(int width, int height) {
	if (app != nullptr) app->framebufferResizeEvent(width, height);
}

void Core::windowResizeEvent(int width, int height) {
	if (app != nullptr) app->windowResizeEvent(width, height);
}