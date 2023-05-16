#include "menuScreen.h"
#include "ui.h"
#include "buttonAction.h"

//extern std::shared_ptr<App> app;

void MenuScreen::init()
{
	// Image
	std::shared_ptr<Image> image = std::make_shared<Image>();
	image->setPosition(glm::vec3(0, Screen::application->getHeight(), -1));
	image->setSize(glm::vec3(Screen::application->getWidth(), Screen::application->getHeight(), 0));
	image->setMaterial("title");
	// Start button
	std::shared_ptr<Button> startButton = std::make_shared<Button>(glm::vec3(215, 210, 0), glm::vec3(190, 60, 0), ButtonAction::loadGame);
	std::shared_ptr<Button> instructionsButton = std::make_shared<Button>(glm::vec3(140, 125, 0), glm::vec3(360, 70, 0), ButtonAction::loadInstruction);

	ui->add(image);
	ui->add(startButton);
	ui->add(instructionsButton);

	Screen::init();

	std::cout << "menu added" << std::endl;
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
	glDisable(GL_DEPTH_TEST);
	Global::graphics.bindShader("ui");
	ui->draw();
	glEnable(GL_DEPTH_TEST);
}