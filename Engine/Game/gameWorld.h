#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

class GameSystem;
class GameObject;

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	// Game system related
	template <class T> bool addGameSystem(std::string tag);  // Add a new game system
	bool addGameSystem(std::string tag, std::shared_ptr<GameSystem> gameSystem);  // Add an existed game system
	bool removeGameSystem(std::string tag);  // remove a game system
	template <class T> std::shared_ptr<T> getGameSystem(std::string tag);  // get a game system

	// Game objects related
	template <class T> bool addGameObject(std::string tag);  // Add a new game object
	bool addGameObject(std::string tag, std::shared_ptr<GameObject> gameObject);  // Add an existed game object
	bool removeGameObject(std::string tag, std::shared_ptr<GameObject> gameObject);  // Remove a game object
	template <class T> std::vector<std::shared_ptr<T>> getGameObjects(std::string tag);  // Get a list of game object

protected:
	std::unordered_map<std::string, std::shared_ptr<GameSystem>> gameSystems;
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> gameObjects;
};

template<class T>
bool GameWorld::addGameSystem(std::string tag)
{
	gameSystems[tag] = std::make_shared<T>();
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

template<class T>
std::vector<std::shared_ptr<T>> GameWorld::getGameObjects(std::string tag)
{
	if (gameObjects.find(tag) == gameObjects.end()) return std::vector<std::shared_ptr<T>>();
	return gameObjects[tag];
}
