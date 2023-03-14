#include "gameScreen.h"

#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/components/drawComponent.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/components/characterMoveComponent.h>
#include <Engine/Game/components/characterJumpComponent.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/components/collisionComponents/cylinderComponent.h>
#include <Engine/Game/components/collisionResponseComponent.h>
#include <Engine/Game/components/physicsComponent.h>

#include "gameComponents/characterCollisionResponse.h"
#include <Game/gameComponents/fallingCollisionResponse.h>
#include <Game/gameComponents/fallingPhysics.h>
#include <Engine/Game/components/collisionComponents/environmentComponent.h>

extern std::shared_ptr<App> app;

GameScreen::GameScreen() 
	: Screen(){

	addEnvironmentMesh("ceiling", "./Resources/Meshes/ceiling.obj");

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.png");
}

void GameScreen::init()
{
	auto camera = std::make_shared<Camera>();
	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this());

	// Create game object
	std::shared_ptr<GameObject> character = createCharacter();
	std::vector<std::shared_ptr<GameObject>> grounds = createGrounds();
	std::shared_ptr<GameObject> environment = createEnvironment("ceiling", "monokuma");

	// Create systems
	drawSystem = std::make_shared<DrawSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();
	std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>(camera, character);
	collisionSystem = std::make_shared<CollisionSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(physicsSystem);
	gameWorld->addGameSystem(characterControllerSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);

	// Add game objects to systems and game world
	// Draw system
	drawSystem->addGameObject(character);
	for(auto ground: grounds) drawSystem->addGameObject(ground);
	drawSystem->addGameObject(environment);
	// Physics system
	physicsSystem->addGameObject(character);
	// Character controller system
	characterControllerSystem->setCharatcer(character);
	// Collision system
	collisionSystem->addGameObject(character);
	collisionSystem->addEnvironmentObject(environment);
	// Game world
	gameWorld->addGameObject(character);
	for(auto ground: grounds) gameWorld->addGameObject(ground);

	score = 0;
	result = "";
	time = 0;
	fallingNumber = 0;
	active = true;

	Screen::init();
}

std::shared_ptr<GameObject> GameScreen::createCharacter()
{
	std::shared_ptr<GameObject> character = std::make_shared<GameObject>("character");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(glm::vec3(0, 1, 0));
	transformComponent->updateRay();
	gameWorld->getCamera()->setPos(modelTransform->getPos());
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monokuma");
	// Physics component
	std::shared_ptr<PhysicsComponent> physicsComponent = std::make_shared<PhysicsComponent>();
	// CharacterMoveComponent
	std::shared_ptr<CharacterMoveComponent> characterMoveComponent = std::make_shared<CharacterMoveComponent>();
	// CharacterJumpComponent
	std::shared_ptr<CharacterJumpComponent> characterJumpComponent = std::make_shared<CharacterJumpComponent>();
	// Collision component
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>();
	// Collision response component
	std::shared_ptr<CharacterCollisionResponse> collisionResponseComponent = std::make_shared<CharacterCollisionResponse>();

	//// Add components to game objects
	character->addComponent(transformComponent);
	character->addComponent(drawComponent);
	character->addComponent(physicsComponent);
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
			std::shared_ptr<GameObject> ground = std::make_shared<GameObject>("ground");
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

std::shared_ptr<GameObject> GameScreen::createFalling(glm::vec3 pos)
{
	std::shared_ptr<GameObject> fallingObject = std::make_shared<GameObject>("falling");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(pos);
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monokuma");
	// Collision component
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>();
	// Collision response component
	std::shared_ptr<FallingCollisionResponse> collisionResponseComponent = std::make_shared<FallingCollisionResponse>();
	// Physics component
	std::shared_ptr<FallingPhysics> fallingPhysics = std::make_shared<FallingPhysics>();

	// Add components to game objects
	fallingObject->addComponent(transformComponent);
	fallingObject->addComponent(drawComponent);
	fallingObject->addComponent(cylinderComponent);
	fallingObject->addComponent(collisionResponseComponent);
	fallingObject->addComponent(fallingPhysics);

	return fallingObject;
}

std::shared_ptr<GameObject> GameScreen::createEnvironment(std::string shape, std::string material)
{
	std::shared_ptr<GameObject> environmentObject = std::make_shared<GameObject>("environment");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	if (shape.compare("ceiling") == 0) {
		modelTransform->scale(0.25);
		modelTransform->translate(glm::vec3(0, 0.5, 0));
	}
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<EnvironmentComponent> collisionComponent = std::make_shared<EnvironmentComponent>(getEnvironmentMesh(shape));

	// Add components to game objects
	environmentObject->addComponent(transformComponent);
	environmentObject->addComponent(drawComponent);
	environmentObject->addComponent(collisionComponent);

	return environmentObject;
}

void GameScreen::update(double seconds) {
	if (keyPressing[GLFW_KEY_B]) {
		app->activateScreen("menu");
		return;
	}
	if (keyPressing[GLFW_KEY_R]) {
		init();
		return;
	}
	if (!active) return;
	time += seconds;
	//if (time >= fallingNumber * 2) {
	//	fallFalling();
	//	fallingNumber++;
	//}
	checkResult();
	if(gameWorld != nullptr)
		gameWorld->update(seconds);
}

void GameScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 0.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong
	if (gameWorld != nullptr)
		gameWorld->draw();

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back.", glm::ivec2(0, 30), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.3f, 0.1f, glm::vec3(1, 0, 1));
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), result, glm::ivec2(50, 300), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.5f, 0.1f, glm::vec3(1, 0, 1));
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Score: " + std::to_string(score), glm::ivec2(0, 440), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Coming in time: " + std::to_string(std::max(0.0f, maxTime - time)) + "s", glm::ivec2(0, 470), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}

void GameScreen::fallFalling()
{
	int x, z;
	do {
		x = rand() % 8 - 4, z = rand() % 8 - 4;
	} while (x == 0 && z == 0);
	auto fallingObject = createFalling(glm::vec3(x, 6, z));
	drawSystem->addGameObject(fallingObject);
	physicsSystem->addGameObject(fallingObject);
	collisionSystem->addGameObject(fallingObject);
	gameWorld->addGameObject(fallingObject);
}

void GameScreen::addScore()
{
	score++;
}

void GameScreen::checkResult()
{
	if (score >= 3) {
		active = false;
		result = "You win!";
		return;
	}
	if (time > maxTime) {
		active = false;
		result = "You lose!";
		return;
	}
}