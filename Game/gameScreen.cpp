#include "gameScreen.h"

#include "Engine/Game/gameSystems/drawSystem.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/components/drawComponent.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/components/characterMoveComponent.h>
#include <Engine/Game/components/characterJumpComponent.h>

extern std::shared_ptr<App> app;

GameScreen::GameScreen() : 
	Screen(),
	mouseStateRight(false) {

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.png");

	init();
}

void GameScreen::init()
{
	gameWorld = std::make_shared<GameWorld>(camera);

	// Create systems
	std::shared_ptr<DrawSystem> drawSystem = std::make_shared<DrawSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(characterControllerSystem);

	// Create game object
	std::shared_ptr<GameObject> character = createCharacter();
	std::vector<std::shared_ptr<GameObject>> grounds = createGrounds();

	// Add game objects to systems and game world
	drawSystem->addGameObject(character);
	for(auto ground: grounds) drawSystem->addGameObject(ground);
	characterControllerSystem->setCharatcer(character);
	gameWorld->addGameObject("character", character);
	for(auto ground: grounds) gameWorld->addGameObject("ground", ground);
}

std::shared_ptr<GameObject> GameScreen::createCharacter()
{
	std::shared_ptr<GameObject> character = std::make_shared<GameObject>();

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(glm::vec3(0, 0.5 / 4, 0));
	gameWorld->getCamera()->setPos(modelTransform->getPos());
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>();
	drawComponent->setShape("cylinder");
	drawComponent->setMaterial("monokuma");
	// CharacterMoveComponent
	std::shared_ptr<CharacterMoveComponent> characterMoveComponent = std::make_shared<CharacterMoveComponent>();
	// CharacterJumpComponent
	std::shared_ptr<CharacterJumpComponent> characterJumpComponent = std::make_shared<CharacterJumpComponent>();

	// Add components to game objects
	character->addComponent(transformComponent);
	character->addComponent(drawComponent);
	character->addComponent(characterMoveComponent);
	character->addComponent(characterJumpComponent);

	return character;
}

std::vector<std::shared_ptr<GameObject>> GameScreen::createGrounds()
{
	std::vector<std::shared_ptr<GameObject>> grounds;

	int xNum = 8, zNum = 8;

	for (int x = 0; x < xNum; x++) {
		for (int z = 0; z < zNum; z++) {
			std::shared_ptr<GameObject> ground = std::make_shared<GameObject>();
			// DrawComponent
			ground->addComponent(std::make_shared<DrawComponent>("quad", "grass"));
			// TransformComponent
			std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
			auto modelTransform = transformComponent->getModelTransform();
			modelTransform->translate(glm::vec3(x - xNum / 2, 0, z - zNum / 2));
			ground->addComponent(transformComponent);

			grounds.push_back(ground);
		}
	}

	return grounds;
}

GameScreen::~GameScreen() {

}

void GameScreen::update(double seconds) {

	gameWorld->update(seconds);

}

void GameScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 0.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong
	gameWorld->draw();

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back.", glm::ivec2(0, 30), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}

void GameScreen::keyEvent(int key, int action) {
	if (action == GLFW_PRESS) {
		keyPressing[key] = true;
		switch (key) {
		case GLFW_KEY_B:
			app->activateScreen("menu");
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		keyPressing[key] = false;
	}
}

void GameScreen::mousePosEvent(double xpos, double ypos) {
	float rotateSpeed = 0.005f;
	
	auto look = camera->getLook();
	if (mouseStateRight) {
		if (firstPerson) {
			camera->rotate(rotateSpeed * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
		}
		else {
			auto characterList = gameWorld->getGameObjects("character");
			if (characterList.size() != 0) {
				auto character = characterList[0];

				camera->rotate(rotateSpeed * (previousMousePosition.x - xpos), glm::vec3(0, 1, 0));
				camera->rotate(rotateSpeed * (previousMousePosition.y - ypos), glm::vec3(look.z, 0, look.x));

				auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();
				auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
				camera->setPos(characterPosition - camera->getLook() * (glm::length(characterPosition - cameraPosition)));
			}
		}
	}
	previousMousePosition = { xpos, ypos };
}

void GameScreen::mouseButtonEvent(int button, int action) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseStateRight = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseStateRight = false;
			break;
		}
	}
}

void GameScreen::scrollEvent(double distance) {
	auto characterList = gameWorld->getGameObjects("character");
	if (characterList.size() == 0) return;

	auto character = characterList[0];
	auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();

	auto oldPosition = camera->getPos(), look = camera->getLook(), characterPosition = characterModelTransform->getPos();
	auto newPosition = oldPosition + look * (float)distance;
	if (firstPerson) {
		if (distance >= 0) newPosition = characterPosition;
		else firstPerson = false;
	}
	else {
		if (glm::dot(characterPosition - oldPosition, characterPosition - newPosition) <= 0) {
			newPosition = characterPosition;
			firstPerson = true;
		}
	}
	camera->setPos(newPosition);
}

