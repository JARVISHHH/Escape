#include "application.h"
#include "screen.h"

Application::Application() {
	
}

Application::~Application() {

}

void Application::start() {

}

void Application::addScreen(std::string name, std::shared_ptr<Screen> screen) {
	screens[name] = screen;
}

void Application::activateScreen(std::string name) {
	if (screens.find(name) == screens.end()) {
		std::cerr << "Screen with name " << name << " does not exist." << std::endl;
	}
	screens[name]->init();
	screens[name]->windowResizeEvent(width, height);
	activeScreen = screens[name];
}

void Application::update(double seconds) {
	if (activeScreen != nullptr) {
		activeScreen->update(seconds);
	}
}

void Application::draw() {
	if (activeScreen != nullptr) activeScreen->draw();
}

void Application::keyEvent(int key, int action) {
	if (activeScreen != nullptr) activeScreen->keyEvent(key, action);
}

void Application::mousePosEvent(double xpos, double ypos) {
	if (activeScreen != nullptr) activeScreen->mousePosEvent(xpos, ypos);
}

void Application::mouseButtonEvent(int button, int action) {
	if (activeScreen != nullptr) activeScreen->mouseButtonEvent(button, action);
}

void Application::scrollEvent(double distance) {
	if (activeScreen != nullptr) activeScreen->scrollEvent(distance);
}

void Application::framebufferResizeEvent(int width, int height) {
	if (activeScreen != nullptr) activeScreen->framebufferResizeEvent(width, height);
}

void Application::windowResizeEvent(int width, int height) {
	if (activeScreen != nullptr) activeScreen->windowResizeEvent(width, height);
	this->width = width, this->height = height;
}

int Application::getWidth()
{
	return width;
}

int Application::getHeight()
{
	return height;
}