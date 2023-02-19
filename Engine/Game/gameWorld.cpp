#include "gameWorld.h"

#include "gameSystem.h"
#include "gameObject.h"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

template<class T>
bool GameWorld::addGameSystem(std::string tag)
{
	gameSystems[tag] = std::make_shared<T>();
	return true;
}

bool GameWorld::addGameSystem(std::string tag, std::shared_ptr<GameSystem> gameSystem)
{
	gameSystems[tag] = gameSystem;
	return true;
}

bool GameWorld::removeGameSystem(std::string tag)
{
	gameSystems[tag] = nullptr;
	return true;
}

template<class T>
std::shared_ptr<T> GameWorld::getGameSystem(std::string tag)
{
	if (gameSystems.find(tag) != gameSystems.end()) return gameSystems[tag];
	else return nullptr;
}

template<class T>
bool GameWorld::addGameObject(std::string tag)
{
	if (gameObjects.find(tag) == gameObjects.end()) gameObjects[tag] = std::vector<std::shared_ptr<GameObject>>();
	gameObjects[tag].push_back(std::make_shared<T>());
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

template<class T>
std::vector<std::shared_ptr<T>> GameWorld::getGameObjects(std::string tag)
{
	if (gameObjects.find(tag) == gameObjects.end()) return std::vector<std::shared_ptr<T>>();
	return gameObjects[tag];
}