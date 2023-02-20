#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "Engine/screen.h"

class GameSystem;
class GameObject;

class GameWorld
{
public:
	GameWorld(std::shared_ptr<Camera> camera);

	void update(double seconds);
	void draw();

	// Game system related
	template <class T> bool addGameSystem();  // Add a new game system
	bool addGameSystem(std::shared_ptr<GameSystem> gameSystem);  // Add an existed game system
	bool removeGameSystem(std::string tag);  // remove a game system
	template <class T> std::shared_ptr<T> getGameSystem(std::string tag);  // get a game system

	// Game objects related
	bool addGameObject(std::string tag, std::shared_ptr<GameObject> gameObject);  // Add an existed game object
	bool removeGameObject(std::string tag, std::shared_ptr<GameObject> gameObject);  // Remove a game object
	std::vector<std::shared_ptr<GameObject>> getGameObjects(std::string tag);  // Get a list of game object

	std::shared_ptr<Camera> getCamera();

protected:
	std::shared_ptr<Screen> screen;
	std::unordered_map<std::string, std::shared_ptr<GameSystem>> gameSystems;
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> gameObjects;

	std::shared_ptr<Camera> camera;
};

template<class T>
bool GameWorld::addGameSystem()
{
	std::shared_ptr<T> gameSystem = std::make_shared<T>();
	gameSystems[gameSystem->getTag()] = gameSystem;
	return true;
}

template<class T>
std::shared_ptr<T> GameWorld::getGameSystem(std::string tag)
{
	if (gameSystems.find(tag) != gameSystems.end()) return static_pointer_cast<T>(gameSystems[tag]);
	else return nullptr;
}
