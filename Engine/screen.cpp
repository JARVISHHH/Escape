#include "screen.h"
#include <Engine/Game/gameWorld.h>
#include <Engine/UIKit/uiElement.h>

std::unordered_map<int, bool> Screen::keyPressing;
glm::vec2 Screen::mousePos(0, 0);
std::unordered_map<int, bool> Screen::mousePressing;
std::unordered_map<std::string, std::vector<std::shared_ptr<Triangle>>> Screen::meshTriangles;

Screen::Screen()
{
	ui = std::make_shared<UIElement>();
	//init();
}

void Screen::init()
{
	initiated = true;
}

void Screen::activate()
{
	windowResizeEvent(640, 480);
	if (!initiated) {
		init();
		initiated = true;
	}
	activateAction();
	windowResizeEvent(application->getWidth(), application->getHeight());
	active = true;
}

void Screen::activateAction()
{
}

std::shared_ptr<Application> Screen::getApp()
{
	return Screen::application;
}

void Screen::addEnvironmentMesh(std::string name, std::string path, bool hasUV, int uvScale)
{
	meshTriangles[name] = std::vector<std::shared_ptr<Triangle>>();
	auto temp = Global::graphics.addShape(name, path, hasUV, uvScale);
	for (int i = 0; i < temp.size(); i += 3) {
		meshTriangles[name].push_back(std::make_shared<Triangle>(temp[i], temp[i + 1], temp[i + 2]));
	}
}

std::vector<std::shared_ptr<Triangle>> Screen::getEnvironmentMesh(std::string name)
{
	if (meshTriangles.find(name) != meshTriangles.end()) return meshTriangles[name];
	else std::cerr << "No mesh named " << name;
	return std::vector<std::shared_ptr<Triangle>>();
}

void Screen::update(double seconds) {
	if (!active) return;
	if (gameWorld != nullptr) gameWorld->update(seconds);
}

void Screen::draw() {

}

void Screen::keyEvent(int key, int action) {
	if (!active) return;
	if (action == GLFW_PRESS) keyPressing[key] = true;
	else if (action == GLFW_RELEASE) keyPressing[key] = false;
}

void Screen::mousePosEvent(double xpos, double ypos) {
	if (!active) return;
	mousePos[0] = xpos, mousePos[1] = ypos;
	if (gameWorld != nullptr) gameWorld->mousePosEvent(xpos, ypos);
	ui->mousePosEvent(xpos, ypos);
}

void Screen::mouseButtonEvent(int button, int action) {
	if (!active) return;
	if (action == GLFW_PRESS) mousePressing[button] = true;
	else if (action == GLFW_RELEASE) mousePressing[button] = false;
	ui->mouseButtonEvent(button, action);
}

void Screen::scrollEvent(double distance) {
	if (gameWorld != nullptr) gameWorld->scrollEvent(distance);
}

void Screen::framebufferResizeEvent(int width, int height) {
	Global::graphics.setFramebufferSize(glm::ivec2(width, height));
	if (gameWorld != nullptr) gameWorld->setFramebufferSize(width, height);
}

void Screen::windowResizeEvent(int width, int height) {
	Global::graphics.setWindowSize(glm::ivec2(width, height));
	if (gameWorld != nullptr) gameWorld->windowResizeEvent(width, height);
	if (ui != nullptr) ui->resize(glm::vec2((float)width / this->width, (float)height / this->height));
	this->width = width, this->height = height;
}