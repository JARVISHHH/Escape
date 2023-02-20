#include "gameWorld.h"

#include "gameSystem.h"
#include "gameObject.h"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

bool GameWorld::addGameSystem(std::shared_ptr<GameSystem> gameSystem)
{
	gameSystems[gameSystem->getTag()] = gameSystem;
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