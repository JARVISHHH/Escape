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

#include "ui.h"
#include "buttonAction.h"

void GameScreen::init()
{
	Screen::init();

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
	std::shared_ptr<GameObject> gameHandler = createGameHandler(gameWorld, 600);

	// Generate map
	std::shared_ptr<Map> map = std::make_shared<Map>(gameWorld->getAABB());
	map->generateMap();
	//map->printMap();
	createDungeon(gameWorld, shared_from_this(), map, gameHandler);
	createLights(gameWorld, map);

	// Collision
	collisionSystem->buildBVH();
	collisionSystem->buildHG();
	//collisionSystem->addLayer("projectile");
	//collisionSystem->deleteLayerCollision("projectile", "enemy");
	//collisionSystem->deleteLayerCollision("projectile", "projectile");
	collisionSystem->deleteLayerCollision("enemy", "enemy");
	collisionSystem->deleteLayerCollision("character", "character");

	gameWorld->start();
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
	Global::graphics.bindShader("ui");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back. Press R to restart.", glm::ivec2(0, 30), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.3f, 0.1f, glm::vec3(1, 0, 1));
}