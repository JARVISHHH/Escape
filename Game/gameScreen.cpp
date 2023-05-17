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
#include <Engine/Game/gameSystems/particleSystem.h>
#include <Game/prefabs/lights.h>

#include "buttonAction.h"

void GameScreen::init()
{
	Screen::init();

	// Back/Restart
	std::shared_ptr<Image> image = std::make_shared<Image>();
	image->setPosition(glm::vec3(width - 80, height - 10, -1));
	image->setSize(glm::vec3(80, 60, 0));
	image->setMaterial("back");
	std::shared_ptr<Button> backButton = std::make_shared<Button>(glm::vec3(width - 75, height - 10, -1), glm::vec3(70, 25, 0), ButtonAction::loadTitle);
	std::shared_ptr<Button> restartButton = std::make_shared<Button>(glm::vec3(width - 85, height - 40, -1), glm::vec3(90, 25, 0), ButtonAction::loadGame);
	// Shoot image
	std::shared_ptr<Image> shootImage = std::make_shared<Image>();
	shootImage->setPosition(glm::vec3((float)width / 2 - 25, (float)height / 2 + 25, -1));
	shootImage->setSize(glm::vec3(50, 50, 0));
	shootImage->setMaterial("shoot");
	// Hp
	std::shared_ptr<Image> hpBarImage = std::make_shared<Image>();
	hpBarImage->setPosition(glm::vec3(15, height + 10, -1));
	hpBarImage->setSize(glm::vec3(120, 80, 0));
	hpBarImage->setMaterial("hp");
	hpImage = std::make_shared<Image>();
	hpImage->setPosition(glm::vec3(35, height + 10, -1));
	hpImage->setSize(glm::vec3(97, 80, 0));
	hpImage->setMaterial("hp_red");
	// Time
	std::shared_ptr<Image> timeBarImage = std::make_shared<Image>();
	timeBarImage->setPosition(glm::vec3(200, height + 18, -1));
	timeBarImage->setSize(glm::vec3(100, 80, 0));
	timeBarImage->setMaterial("time");
	timeImage = std::make_shared<Image>();
	timeImage->setPosition(glm::vec3(240, height + 10, -1));
	timeImage->setSize(glm::vec3(200, 80, 0));
	timeImage->setMaterial("time_blue");
	// Result
	resultImage = std::make_shared<Image>();
	resultImage->setPosition(glm::vec3(100, height, -1));
	resultImage->setSize(glm::vec3(500, 350, 0));
	resultImage->setShow(false);
	scoreImage = std::make_shared<Image>();
	scoreImage->setPosition(glm::vec3(250, height - 300, -1));
	scoreImage->setSize(glm::vec3(100, 120, 0));
	scoreImage->setMaterial("score");
	scoreImage->setShow(false);
	auto score = std::make_shared<Text>();
	score->setLocalPosition(glm::vec3(75, -43, -1));
	score->setFontSize(0.8);
	score->setColor(glm::vec3(1, 1, 1));
	//score->setContent("50");
	score->setShow(false);

	ui->add(image);
	ui->add(backButton);
	ui->add(restartButton);
	ui->add(shootImage);
	ui->add(hpBarImage);
	ui->add(hpImage);
	ui->add(timeBarImage);
	ui->add(timeImage);
	ui->add(resultImage);
	ui->add(scoreImage);
	scoreImage->add(score);
}

void GameScreen::activateAction()
{
	auto camera = std::make_shared<Camera>(width, height);

	gameWorld = std::make_shared<GameWorld>(camera, shared_from_this(), width, height);

	// Create systems
	drawSystem = std::make_shared<DrawSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem = std::make_shared<CharacterControllerSystem>();
	std::shared_ptr<CameraSystem> cameraSystem = std::make_shared<CameraSystem>(camera);
	collisionSystem = std::make_shared<CollisionSystem>(gameWorld, 6);
	std::shared_ptr<TickSystem> tickSystem = std::make_shared<TickSystem>();
	std::shared_ptr<ParticleSystem> particleSystem = std::make_shared<ParticleSystem>();

	// Add systems to game world
	gameWorld->addGameSystem(drawSystem);
	gameWorld->addGameSystem(physicsSystem);
	gameWorld->addGameSystem(characterControllerSystem);
	gameWorld->addGameSystem(cameraSystem);
	gameWorld->addGameSystem(collisionSystem);
	gameWorld->addGameSystem(tickSystem);
	gameWorld->addGameSystem(particleSystem);

	// Create game handler
	std::shared_ptr<GameObject> gameHandler = createGameHandler(gameWorld, 90, timeImage, resultImage, scoreImage);

	// Generate map
	std::shared_ptr<Map> map = std::make_shared<Map>(gameWorld->getAABB());
	map->generateMap();
	//map->printMap();
	createDungeon(gameWorld, shared_from_this(), map, gameHandler);
	createLights(gameWorld, map);

	// Set hp
	gameWorld->getGameObject("character")->getComponent<CharacterHealth>("health")->setHP(hpImage);

	// Collision
	collisionSystem->buildBVH();
	collisionSystem->buildHG();
	collisionSystem->deleteLayerCollision("enemy", "enemy");
	collisionSystem->deleteLayerCollision("character", "character");

	gameWorld->start();

	hpImage->setSize(glm::vec3(97, 80, 0));
	timeImage->setSize(glm::vec3(200, 80, 0));
	resultImage->setSize(glm::vec3(500, 350, 0));
	resultImage->setShow(false);
	scoreImage->setShow(false);
}

void GameScreen::update(double seconds) {
	if (keyPressing[GLFW_KEY_B]) {
		getApp()->activateScreen("menu");
		return;
	}
	if (keyPressing[GLFW_KEY_R]) {
		getApp()->activateScreen("game");
		return;
	}
	if (!active) return;
	if(gameWorld != nullptr)
		gameWorld->update(seconds);
}

void GameScreen::draw() {
	// Phong
	if (gameWorld != nullptr)
		gameWorld->draw();

	// Text
	//Global::graphics.bindShader("ui");
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back. Press R to restart.", glm::ivec2(0, 30), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.3f, 0.1f, glm::vec3(1, 0, 1));

	// UI
	glDisable(GL_DEPTH_TEST);
	Global::graphics.bindShader("ui");
	ui->draw();
	glEnable(GL_DEPTH_TEST);
}