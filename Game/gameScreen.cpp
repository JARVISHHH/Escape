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
#include <Game/gameComponents/movingBoard.h>

#include <corecrt_math_defines.h>

extern std::shared_ptr<App> app;

GameScreen::GameScreen() 
	: Screen(){

	addEnvironmentMesh("level", "./Resources/Meshes/level.obj");
	addEnvironmentMesh("board", "./Resources/Meshes/board.obj");
	addEnvironmentMesh("bvh_test", "./Resources/Meshes/bvh_test.obj");
	//addEnvironmentMesh("square1", "./Resources/Meshes/square1.obj");

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.png");
	Global::graphics.addMaterial("monomi", "Resources/Images/monomi.png");
}

void GameScreen::init()
{
	auto camera = std::make_shared<Camera>();
	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this());

	// Create game object
	std::shared_ptr<GameObject> character = createCharacter();
	//std::vector<std::shared_ptr<GameObject>> grounds = createGrounds();
	std::shared_ptr<GameObject> environment = createEnvironment("level", "grass");
	//std::shared_ptr<GameObject> environment = createEnvironment("bvh_test", "grass");
	//std::shared_ptr<GameObject> environment = createEnvironment("square", "grass");
	//std::shared_ptr<GameObject> environment1 = createEnvironment("square1", "grass");
	//std::shared_ptr<GameObject> movingBoard = createBoard(glm::vec3(0, 3, -9), glm::vec3(0, 3, -15));
	std::shared_ptr<GameObject> goalObject = createGoal(glm::vec3(0, 0.5, 2));

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
	//for(auto ground: grounds) drawSystem->addGameObject(ground);
	drawSystem->addGameObject(environment);
	//drawSystem->addGameObject(environment1);
	//drawSystem->addGameObject(movingBoard);
	drawSystem->addGameObject(goalObject);
	// Physics system
	physicsSystem->addGameObject(character);
	//physicsSystem->addGameObject(movingBoard);
	// Character controller system
	characterControllerSystem->setCharatcer(character);
	// Collision system
	collisionSystem->addGameObject(character);
	collisionSystem->addEnvironmentObject(environment);
	//collisionSystem->addEnvironmentObject(environment1);
	//collisionSystem->addEnvironmentObject(movingBoard);
	collisionSystem->addGameObject(goalObject);
	collisionSystem->buildBVH();
	// Game world
	gameWorld->addGameObject(character);
	//for(auto ground: grounds) gameWorld->addGameObject(ground);
	gameWorld->addGameObject(environment);
	//gameWorld->addGameObject(environment1);
	//gameWorld->addGameObject(movingBoard);
	gameWorld->addGameObject(goalObject);

	// Hierarchical grid test
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 3; k++) {
				std::shared_ptr<GameObject> goalObject = createGoal(glm::vec3(-i, -k * 0.5 - 0.5, -j));
				drawSystem->addGameObject(goalObject);
				collisionSystem->addGameObject(goalObject);
				gameWorld->addGameObject(goalObject);
			}
		}
	}

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
	modelTransform->translate(glm::vec3(0, 3, 0));
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
			transformComponent->updateRay();
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
	transformComponent->updateRay();
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
	else if (shape.compare("level") == 0) {
		modelTransform->scale(0.25);
		//modelTransform->scale(1.2);
	}
	else if (shape.compare("bvh_test") == 0) {
		modelTransform->scale(0.25);
	}
	else if (shape.compare("square") == 0) {

	}
	else if (shape.compare("square1") == 0) {
		modelTransform->translate(glm::vec3(-5, -0.5, 0));
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

std::shared_ptr<GameObject> GameScreen::createBoard(glm::vec3 start, glm::vec3 end)
{
	std::shared_ptr<GameObject> boardObject = std::make_shared<GameObject>("board");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->rotate(M_PI / 2, glm::vec3(0, 1, 0));
	modelTransform->scale(0.25);
	modelTransform->translate(start);
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("board");
	// Collision component
	std::shared_ptr<EnvironmentComponent> collisionComponent = std::make_shared<EnvironmentComponent>(getEnvironmentMesh("board"));
	// Physics component
	std::shared_ptr<MovingBoard> movingBoard = std::make_shared<MovingBoard>(2.5, start + 1.5f * 0.25f * glm::normalize(end - start), end + 1.5f * 0.25f * glm::normalize(start - end));

	// Add components to game objects
	boardObject->addComponent(transformComponent);
	boardObject->addComponent(drawComponent);
	boardObject->addComponent(collisionComponent);
	boardObject->addComponent(movingBoard);

	return boardObject;
}

std::shared_ptr<GameObject> GameScreen::createGoal(glm::vec3 pos)
{
	std::shared_ptr<GameObject> goalObject = std::make_shared<GameObject>("goal");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(pos);
	transformComponent->updateRay();
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monomi");
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<CollisionResponseComponent> collisionResponse = std::make_shared<CollisionResponseComponent>(true);

	// Add components to game objects
	goalObject->addComponent(transformComponent);
	goalObject->addComponent(drawComponent);
	goalObject->addComponent(collisionComponent);
	goalObject->addComponent(collisionResponse);

	return goalObject;
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
	//time += seconds;
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
	Global::graphics.setClearColor(glm::vec3(0.0f, 0.2f, 0.8f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong
	if (gameWorld != nullptr)
		gameWorld->draw();

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back. Press R to restart.", glm::ivec2(0, 30), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.3f, 0.1f, glm::vec3(1, 0, 1));
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), result, glm::ivec2(50, 300), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.5f, 0.1f, glm::vec3(1, 0, 1));
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Score: " + std::to_string(score), glm::ivec2(0, 440), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Coming in time: " + std::to_string(std::max(0.0f, maxTime - time)) + "s", glm::ivec2(0, 470), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}

void GameScreen::fallFalling()
{
	int x, z;
	do {
		x = rand() % 8 - 4, z = rand() % 8 - 4;
	} while (x == 0 && z == 0);
	auto fallingObject = createFalling(glm::vec3(x, 5, z));
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
	if (gameWorld->isFinish() && gameWorld->istWin()) {
		//active = false;
		result = "You win!";
		return;
	}
	//if (score >= 3) {
	//	active = false;
	//	result = "You win!";
	//	return;
	//}
	//if (time > maxTime) {
	//	active = false;
	//	result = "You lose!";
	//	return;
	//}
}