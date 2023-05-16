#include "menuScreen.h"
#include "ui.h"

//extern std::shared_ptr<App> app;

void MenuScreen::init()
{
	Screen::init();

	// Text
	// Title text
	std::shared_ptr<Text> titleText = std::make_shared<Text>();
	titleText->setContent("Escape");
	titleText->setLocalPosition(glm::vec3(175, 350, 0));
	titleText->setFontSize(2.0f);
	// Start text
	std::shared_ptr<Text> startText = std::make_shared<Text>();
	startText->setContent("Start");
	startText->setLocalPosition(glm::vec3(0, 0, 0));
	startText->setFontSize(0.5f);
	// Button
	// Start button
	std::shared_ptr<Button> startButton = std::make_shared<Button>(glm::vec3(175, 100, 0), glm::vec3(50, 50, 0), MenuScreen::activeGame);
	// Add to UI
	ui->add(titleText);
	ui->add(startButton);
	startButton->add(startText);
}

void MenuScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 1.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Text
	//Global::graphics.bindShader("text");
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Search", glm::ivec2(175, 350), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.0f, 0.1f, glm::vec3(0, 0, 1));
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press any key to start...", glm::ivec2(175, 100), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(0, 0, 1));

	// UI
	ui->draw();
}

void MenuScreen::keyEvent(int key, int action) {
	Screen::keyEvent(key, action);
	if (action == GLFW_PRESS) {
		activeGame();
	}
}

void MenuScreen::activeGame()
{
	getApp()->activateScreen("game");
}