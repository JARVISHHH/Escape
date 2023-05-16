#include "menuScreen.h"
#include "ui.h"
#include "buttonAction.h"
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/map/map.h>
#include <Game/prefabs/dungeon.h>
#include <Game/prefabs/lights.h>

//extern std::shared_ptr<App> app;

void MenuScreen::init()
{
	Screen::init();

	// Image
	std::shared_ptr<Image> image = std::make_shared<Image>();
	image->setPosition(glm::vec3(0, Screen::application->getHeight(), -1));
	image->setSize(glm::vec3(Screen::application->getWidth(), Screen::application->getHeight(), 0));
	image->setMaterial("title");
	// Start button
	std::shared_ptr<Button> startButton = std::make_shared<Button>(glm::vec3(215, 210, 0), glm::vec3(190, 60, 0), ButtonAction::loadGame);
	std::shared_ptr<Button> instructionsButton = std::make_shared<Button>(glm::vec3(140, 125, 0), glm::vec3(360, 70, 0), ButtonAction::loadInstruction);
	// Add to ui
	ui->add(image);
	ui->add(startButton);
	ui->add(instructionsButton);

	// Create background animation
	auto camera = std::make_shared<Camera>(width, height);
	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this(), width, height);
	auto drawSystem = std::make_shared<DrawSystem>();
	auto physicsSystem = std::make_shared<PhysicsSystem>();
	auto cameraSystem = std::make_shared<CameraSystem>(camera);
	auto collisionSystem = std::make_shared<CollisionSystem>(gameWorld, 6);
	auto tickSystem = std::make_shared<TickSystem>();
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(physicsSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);
	gameWorld->addGameSystem(tickSystem);

	// Generate map
	std::shared_ptr<Map> map = std::make_shared<Map>(gameWorld->getAABB());
	map->generateMap();
	//map->printMap();
	createDungeon(gameWorld, shared_from_this(), map, nullptr);
	createLights(gameWorld, map);

	// Collision
	collisionSystem->buildBVH();
	collisionSystem->buildHG();

	gameWorld->start();
}

void MenuScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 1.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Game
	if (gameWorld != nullptr)
		gameWorld->draw();

	// UI
	glDisable(GL_DEPTH_TEST);
	Global::graphics.bindShader("ui");
	ui->draw();
	glEnable(GL_DEPTH_TEST);
}