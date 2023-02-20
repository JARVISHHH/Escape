#include "menuScreen.h"

extern std::shared_ptr<App> app;

void MenuScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 1.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press any key to start...", glm::ivec2(0, 200), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.f, 0.1f, glm::vec3(0, 0, 1));
}

void MenuScreen::keyEvent(int key, int action) {
	Screen::keyEvent(key, action);
	if (action == GLFW_PRESS) {
		app->activateScreen("game");
	}
}