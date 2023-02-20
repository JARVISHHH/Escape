#include "gameWorld.h"

#include "gameSystem.h"
#include "gameObject.h"
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>


GameWorld::GameWorld(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void GameWorld::update(double seconds)
{
	auto characterControllerSystem = getGameSystem<CharacterControllerSystem>("characterController");
	if (characterControllerSystem != nullptr) characterControllerSystem->update(seconds);
}

void GameWorld::draw()
{
	auto drawSystem = getGameSystem<DrawSystem>("draw");
	if (drawSystem != nullptr) drawSystem->draw();
}

bool GameWorld::addGameSystem(std::shared_ptr<GameSystem> gameSystem)
{
	gameSystems[gameSystem->getTag()] = gameSystem;
	gameSystem->setGameWorld(std::shared_ptr<GameWorld>(this));
	return true;
}

bool GameWorld::removeGameSystem(std::string tag)
{
	gameSystems[tag] = nullptr;
	return true;
}

bool GameWorld::addGameObject(std::string tag, std::shared_ptr<GameObject> gameObject)
{
	if (gameObjects.find(tag) == gameObjects.end()) gameObjects[tag] = std::vector<std::shared_ptr<GameObject>>();
	gameObjects[tag].push_back(gameObject);
	gameObject->setGameWorld(std::shared_ptr<GameWorld>(this));
	return true;
}

bool GameWorld::removeGameObject(std::string tag, std::shared_ptr<GameObject> gameObject)
{
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