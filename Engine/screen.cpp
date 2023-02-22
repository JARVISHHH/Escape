#include "screen.h"
#include <Engine/Game/gameWorld.h>

std::unordered_map<int, bool> Screen::keyPressing;
std::unordered_map<int, bool> Screen::mousePressing;

void Screen::init()
{
	windowResizeEvent(width, height);
}

void Screen::update(double seconds) {
	if (!active) return;
	if (gameWorld != nullptr) gameWorld->update(seconds);
}

void Screen::draw() {
	
}

void Screen::keyEvent(int key, int action) {
	if (action == GLFW_PRESS) keyPressing[key] = true;
	else if (action == GLFW_RELEASE) keyPressing[key] = false;
}

void Screen::mousePosEvent(double xpos, double ypos) {
	if (!active) return;
	if (gameWorld != nullptr) gameWorld->mousePosEvent(xpos, ypos);
}

void Screen::mouseButtonEvent(int button, int action) {
	if (action == GLFW_PRESS) mousePressing[button] = true;
	else if (action == GLFW_RELEASE) mousePressing[button] = false;
}

void Screen::scrollEvent(double distance) {
	if (gameWorld != nullptr) gameWorld->scrollEvent(distance);
}

void Screen::framebufferResizeEvent(int width, int height) {
	Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void Screen::windowResizeEvent(int width, int height) {
	Global::graphics.setWindowSize(glm::ivec2(width, height));
	if (gameWorld != nullptr) gameWorld->windowResizeEvent(width, height);
	this->width = width, this->height = height;
}