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

private:
	std::unordered_map<std::string, std::shared_ptr<GameSystem>> gameSystems;
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> gameObjects;
};
