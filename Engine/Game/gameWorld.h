#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "Engine/screen.h"

class GameSystem;
class GameObject;
class GameComponent;
class AABB;

class GameWorld : public std::enable_shared_from_this<GameWorld>
{
public:
	GameWorld(std::shared_ptr<Camera> camera, std::shared_ptr<Screen> screen);

	void start();
	void update(double seconds);
	void draw();
	void mousePosEvent(double xpos, double ypos);
	void scrollEvent(double distance);
	void windowResizeEvent(int width, int height);

	// Game system related
	template <class T> bool addGameSystem();  // Add a new game system
	bool addGameSystem(std::shared_ptr<GameSystem> gameSystem);  // Add an existed game system
	bool removeGameSystem(std::string tag);  // remove a game system
	template <class T> std::shared_ptr<T> getGameSystem(std::string tag);  // get a game system

	// Game objects related
	bool addGameObject(std::shared_ptr<GameObject> gameObject);  // Add an existed game object
	bool removeGameObject(std::shared_ptr<GameObject> gameObject);  // Remove a game object
	std::shared_ptr<GameObject> getGameObject(std::string tag);  // Get the first found game object
	std::vector<std::shared_ptr<GameObject>> getGameObjects(std::string tag);  // Get a list of game object

	std::shared_ptr<Camera> getCamera();

	std::shared_ptr<Screen> getScreen();

	std::shared_ptr<AABB> getAABB();

	void setIsStop(bool stop) { this->stop = stop; }
	void setWin(bool win) { this->win = win; }
	void setFinish(bool finish) { this->finish = finish; }
	bool isStop() { return stop; }
	bool isFinish() { return finish; }
	bool istWin() { return win; }

protected:
	std::shared_ptr<Screen> screen;
	std::unordered_map<std::string, std::shared_ptr<GameSystem>> gameSystems;
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> gameObjects;

	std::shared_ptr<Camera> camera;

	std::shared_ptr<AABB> aabb = std::make_shared<AABB>(glm::vec4(20, 20, 20, 1), glm::vec4(-20, -5, -20, 1));

	double maxTimeStep = 0.005;
	bool stop = false;

	bool finish = false;
	bool win = false;
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
