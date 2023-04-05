#include "gameWorld.h"

#include "gameSystem.h"
#include "gameObject.h"
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/physicsSystem.h>
#include <Engine/Game/collision/hierarchicalGrid.h>


GameWorld::GameWorld(std::shared_ptr<Camera> camera, std::shared_ptr<Screen> screen)
{
	this->camera = camera;
	this->screen = screen;
}

void GameWorld::update(double seconds)
{
	auto characterControllerSystem = getGameSystem<CharacterControllerSystem>("characterController");
	if (characterControllerSystem != nullptr) characterControllerSystem->update(seconds);
	auto physicsSystem = getGameSystem<PhysicsSystem>("physics");
	if (physicsSystem != nullptr) physicsSystem->update(seconds);
	auto collisionSystem = getGameSystem<CollisionSystem>("collision");
	if (collisionSystem != nullptr) collisionSystem->update(seconds);
	auto cameraSystem = getGameSystem<CameraSystem>("camera");
	if (cameraSystem != nullptr) cameraSystem->update(seconds);
}

void GameWorld::draw()
{
	auto drawSystem = getGameSystem<DrawSystem>("draw");
	if (drawSystem != nullptr) drawSystem->draw();
}

void GameWorld::mousePosEvent(double xpos, double ypos)
{
	auto cameraSystem = getGameSystem<CameraSystem>("camera");
	if (cameraSystem != nullptr) cameraSystem->mousePosEvent(xpos, ypos);
}

void GameWorld::scrollEvent(double distance)
{
	auto cameraSystem = getGameSystem<CameraSystem>("camera");
	if (cameraSystem != nullptr) cameraSystem->scrollEvent(distance);
}

void GameWorld::windowResizeEvent(int width, int height)
{
	auto cameraSystem = getGameSystem<CameraSystem>("camera");
	if (cameraSystem != nullptr) cameraSystem->windowResizeEvent(width, height);
}

bool GameWorld::addGameSystem(std::shared_ptr<GameSystem> gameSystem)
{
	gameSystems[gameSystem->getTag()] = gameSystem;
	gameSystem->setGameWorld(shared_from_this());
	return true;
}

bool GameWorld::removeGameSystem(std::string tag)
{
	gameSystems[tag] = nullptr;
	return true;
}

bool GameWorld::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	std::string tag = gameObject->getTag();
	if (gameObjects.find(tag) == gameObjects.end()) gameObjects[tag] = std::vector<std::shared_ptr<GameObject>>();
	gameObjects[tag].push_back(gameObject);
	gameObject->setGameWorld(std::shared_ptr<GameWorld>(this));
	return true;
}

bool GameWorld::removeGameObject(std::shared_ptr<GameObject> gameObject)
{
	std::string tag = gameObject->getTag();
	if (gameObjects.find(tag) == gameObjects.end()) return true;
	for (int i = 0; i < gameObjects[tag].size(); i++) {
		if (gameObjects[tag][i] == gameObject)
			gameObjects[tag].erase(gameObjects[tag].begin() + i);
	}
	return true;
}

std::vector<std::shared_ptr<GameObject>> GameWorld::getGameObjects(std::string tag) {
	if (gameObjects.find(tag) == gameObjects.end()) return std::vector<std::shared_ptr<GameObject>>();
	return gameObjects[tag];
}

std::shared_ptr<Camera> GameWorld::getCamera()
{
	return camera;
}

std::shared_ptr<Screen> GameWorld::getScreen()
{
	return screen;
}

std::shared_ptr<AABB> GameWorld::getAABB()
{
	return aabb;
}