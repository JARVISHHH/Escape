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
#include <Engine/Game/components/behaviorComponent.h>
#include <Engine/Game/gameSystems/tickSystem.h>

#include "gameComponents/characterCollisionResponse.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>

#include <Game/prefabs/character.h>
#include <Game/prefabs/environment.h>
#include <Game/prefabs/goal.h>
#include <Game/prefabs/enemy.h>

#include <corecrt_math_defines.h>

extern std::shared_ptr<App> app;

GameScreen::GameScreen() 
	: Screen(){
	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.png");
	Global::graphics.addMaterial("monomi", "Resources/Images/monomi.png");
	Global::graphics.addMaterial("learn", "Resources/Images/learn.jpg");
	Global::graphics.addMaterial("wall", "Resources/Images/wall.jpg");
	Global::graphics.addMaterial("ground", "Resources/Images/ground.png");

	addEnvironmentMesh("plane", "./Resources/Meshes/plane.obj");
	addEnvironmentMesh("building", "./Resources/Meshes/building.obj");
	addEnvironmentMesh("wall", "./Resources/Meshes/wall.obj");
	addEnvironmentMesh("test", "./Resources/Meshes/environment3.obj");
	addEnvironmentMesh("map", "./Resources/Meshes/map.obj");
	addEnvironmentMesh("ground", "./Resources/Meshes/groundNav.obj");
}

void GameScreen::init()
{
	auto camera = std::make_shared<Camera>();
	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this());
	
	// Create NavMesh
	navMesh = std::make_shared<NavMesh>("./Resources/Meshes/groundNav.obj");
	navMesh->bake();

	// Create game object
	std::shared_ptr<GameObject> character = createCharacter(gameWorld);
	//std::shared_ptr<GameObject> goalObject = createGoal(glm::vec3(10, 1, 5));
	std::shared_ptr<GameObject> goalObject = createGoal(glm::vec3(11, 5, 0));
	std::shared_ptr<GameObject> environment = createEnvironment(shared_from_this(), "ground");
	std::shared_ptr<GameObject> ground = createEnvironment(shared_from_this(), "ground", "");
	std::shared_ptr<GameObject> enemy = createEnemy("cylinder", "monokuma", glm::vec3(4, 0.5, -3), navMesh);

	// Create systems
	drawSystem = std::make_shared<DrawSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();
	std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>(camera, character);
	collisionSystem = std::make_shared<CollisionSystem>(2, gameWorld);
	std::shared_ptr<TickSystem> tickSystem = std::make_shared<TickSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(physicsSystem);
	gameWorld->addGameSystem(characterControllerSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);
	gameWorld->addGameSystem(tickSystem);

	// Add game objects to systems and game world
	drawSystem->addGameObject(character);
	physicsSystem->addGameObject(character);
	characterControllerSystem->setCharatcer(character);
	collisionSystem->addGameObject(character);
	tickSystem->addComponent(character->getComponent<HealthComponent>("health"));
	gameWorld->addGameObject(character);

	drawSystem->addGameObject(goalObject);	
	collisionSystem->addGameObject(goalObject);
	gameWorld->addGameObject(goalObject);

	drawSystem->addGameObject(ground);
	collisionSystem->addEnvironmentObject(ground);
	gameWorld->addGameObject(ground);

	drawSystem->addGameObject(environment);
	collisionSystem->addEnvironmentObject(environment);
	gameWorld->addGameObject(environment);

	drawSystem->addGameObject(enemy);
	//tickSystem->addComponent(enemy->getComponent<EnemyMovement>("enemyMovement"));
	tickSystem->addComponent(enemy->getComponent<PathfindingComponent>("pathfinding"));
	tickSystem->addComponent(enemy->getComponent<BehaviorComponent>("behavior"));
	collisionSystem->addGameObject(enemy);
	gameWorld->addGameObject(enemy);

	collisionSystem->buildBVH();
	collisionSystem->buildHG();

	gameWorld->start();

	score = 0;
	result = "";
	time = 0;
	fallingNumber = 0;
	active = true;

	Screen::init();
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


void GameScreen::addScore()
{
	score++;
}

void GameScreen::checkResult()
{
	if (gameWorld->isFinish()) {
		active = false;
		if(gameWorld->istWin()) result = "You win!";
		else result = "You lose!";
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