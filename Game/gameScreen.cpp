#include "gameScreen.h"

#include "Engine/Game/gameSystems/drawSystem.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/components/drawComponent.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/components/characterMoveComponent.h>
#include <Engine/Game/components/characterJumpComponent.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/components/collisionComponents/cylinderComponent.h>
#include <Engine/Game/components/collisionResponseComponent.h>

extern std::shared_ptr<App> app;

GameScreen::GameScreen() 
	: Screen(){

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.png");

	init();
}

void GameScreen::init()
{
	auto camera = std::make_shared<Camera>();
	gameWorld = std::make_shared<GameWorld>(camera);

	// Create game object
	std::shared_ptr<GameObject> character = createCharacter();
	std::vector<std::shared_ptr<GameObject>> grounds = createGrounds();
	std::shared_ptr<GameObject> fallingObject = createFalling();

	// Create systems
	std::shared_ptr<DrawSystem> drawSystem = std::make_shared<DrawSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();
	std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>(camera, character);
	std::shared_ptr<CollisionSystem> collisionSystem = std::make_shared<CollisionSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(characterControllerSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);

	// Add game objects to systems and game world
	// Draw system
	drawSystem->addGameObject(character);
	for(auto ground: grounds) drawSystem->addGameObject(ground);
	drawSystem->addGameObject(fallingObject);
	// Character controller system
	characterControllerSystem->setCharatcer(character);
	// Collision system
	collisionSystem->addGameObject(character);
	collisionSystem->addGameObject(fallingObject);
	// Game world
	gameWorld->addGameObject("character", character);
	for(auto ground: grounds) gameWorld->addGameObject("ground", ground);
	gameWorld->addGameObject("falling", fallingObject);
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
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monokuma");
	// CharacterMoveComponent
	std::shared_ptr<CharacterMoveComponent> characterMoveComponent = std::make_shared<CharacterMoveComponent>();
	// CharacterJumpComponent
	std::shared_ptr<CharacterJumpComponent> characterJumpComponent = std::make_shared<CharacterJumpComponent>();
	// Collision component
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>();
	// Collision response component
	std::shared_ptr<CollisionResponseComponent> collisionResponseComponent = std::make_shared<CollisionResponseComponent>();

	// Add components to game objects
	character->addComponent(transformComponent);
	character->addComponent(drawComponent);
	character->addComponent(characterMoveComponent);
	character->addComponent(characterJumpComponent);
	character->addComponent(cylinderComponent);
	character->addComponent(collisionResponseComponent);

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

std::shared_ptr<GameObject> GameScreen::createFalling()
{
	std::shared_ptr<GameObject> fallingObject = std::make_shared<GameObject>();

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(glm::vec3(3, 0.5 / 4, 3));
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monokuma");
	// Collision component
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>();
	// Collision response component
	std::shared_ptr<CollisionResponseComponent> collisionResponseComponent = std::make_shared<CollisionResponseComponent>(true);

	// Add components to game objects
	fallingObject->addComponent(transformComponent);
	fallingObject->addComponent(drawComponent);
	fallingObject->addComponent(cylinderComponent);
	fallingObject->addComponent(collisionResponseComponent);

	return fallingObject;
}

void GameScreen::update(double seconds) {
	if (keyPressing[GLFW_KEY_B]) app->activateScreen("menu");
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