#include "gameScreen.h"

#include <Engine/Game/map/map.h>

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
#include <Game/prefabs/gamehandler.h>
#include <Game/prefabs/dungeon.h>

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

	addEnvironmentMesh("building", "./Resources/Meshes/building.obj");
	addEnvironmentMesh("wall", "./Resources/Meshes/wall.obj");
	addEnvironmentMesh("map", "./Resources/Meshes/map.obj");
	addEnvironmentMesh("ground", "./Resources/Meshes/ground.obj");
	addEnvironmentMesh("plane", "./Resources/Meshes/plane.obj");
	addEnvironmentMesh("box", "./Resources/Meshes/box.obj");
	addEnvironmentMesh("spike", "./Resources/Meshes/spike.obj");
	addEnvironmentMesh("test", "./Resources/Meshes/test.obj");
}

void GameScreen::init()
{
	auto camera = std::make_shared<Camera>();

	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this());

	// Create systems
	drawSystem = std::make_shared<DrawSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();
	std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>(camera);
	collisionSystem = std::make_shared<CollisionSystem>(gameWorld, 6);
	std::shared_ptr<TickSystem> tickSystem = std::make_shared<TickSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(physicsSystem);
	gameWorld->addGameSystem(characterControllerSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);
	gameWorld->addGameSystem(tickSystem);

	// Create game handler
	std::shared_ptr<GameObject> gameHandler = createGameHandler(gameWorld, 600);

	// Generate map
	std::shared_ptr<Map> map = std::make_shared<Map>(gameWorld->getAABB());
	map->generateMap();
	//map->printMap();
	createDungeon(gameWorld, shared_from_this(), map, gameHandler);
	
	// Create NavMesh
	//navMesh = std::make_shared<NavMesh>("./Resources/Meshes/groundNav.obj");
	//navMesh->bake();

	// Create game object
	
	//std::shared_ptr<GameObject> character = createCharacter(gameWorld, glm::vec3(0, 3, 0), gameHandler);
	//std::shared_ptr<GameObject> goalObject = createGoal(gameWorld, glm::vec3(11, 5, 0));
	//std::shared_ptr<GameObject> environment = createEnvironment(gameWorld, shared_from_this(), "test", "grass");
	//auto transform = std::shared_ptr<ModelTransform>();
	//transform->translate
	//std::shared_ptr<GameObject> ground = createEnvironment(gameWorld, shared_from_this(), "box", "grass");
	//std::shared_ptr<GameObject> ground = createEnvironment(gameWorld, shared_from_this(), "ground", "ground");
	//std::shared_ptr<GameObject> chasingEnemy = createChasingEnemy(gameWorld, "cylinder", "monokuma", glm::vec3(4, 0.5, -3), navMesh);
	//std::shared_ptr<GameObject> shootingEnemy = createShootingEnemy(gameWorld, "cylinder", "monokuma", glm::vec3(2, 0.5, -3));

	collisionSystem->buildBVH();
	collisionSystem->buildHG();
	//collisionSystem->addLayer("projectile");
	//collisionSystem->deleteLayerCollision("projectile", "enemy");
	//collisionSystem->deleteLayerCollision("projectile", "projectile");
	collisionSystem->deleteLayerCollision("enemy", "enemy");

	gameWorld->start();

	//score = 0;
	//result = "";
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
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), result, glm::ivec2(50, 300), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.5f, 0.1f, glm::vec3(1, 0, 1));
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Score: " + std::to_string(score), glm::ivec2(0, 440), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Coming in time: " + std::to_string(std::max(0.0f, maxTime - time)) + "s", glm::ivec2(0, 470), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}