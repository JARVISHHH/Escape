#include "instructionScreen.h"
#include "ui.h"
#include "buttonAction.h"

void InstructionScreen::init()
{
	// Image
	std::shared_ptr<Image> image = std::make_shared<Image>();
	image->setPosition(glm::vec3(0, height, -1));
	image->setSize(glm::vec3(width, height, 0));
	image->setMaterial("instructions");
	// Back button
	std::shared_ptr<Button> backButton = std::make_shared<Button>(glm::vec3(width - 90, height - 10, 0), glm::vec3(66, 30, 0), ButtonAction::loadTitle);

	ui->add(image);
	ui->add(backButton);

	Screen::init();
}

void InstructionScreen::draw()
{
	// Set background color
	Global::graphics.setClearColor(glm::vec3(1.0f, 1.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// UI
	glDisable(GL_DEPTH_TEST);
	Global::graphics.bindShader("ui");
	ui->draw();
	glEnable(GL_DEPTH_TEST);
}