#include "gameWorld.h"

#include "gameSystem.h"
#include "gameObject.h"
#include "gameComponent.h"
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/physicsSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>
#include <Engine/Game/collision/hierarchicalGrid.h>
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/gameSystems/particleSystem.h>


GameWorld::GameWorld(std::shared_ptr<Camera> camera, std::shared_ptr<Screen> screen, int frameBufferWidth, int frameBufferHeight)
	:camera(camera), screen(screen), frameBufferWidth(frameBufferWidth), frameBufferHeight(frameBufferHeight)
{

}

GameWorld::~GameWorld()
{
	//std::cout << "game world delete" << std::endl;
}

void GameWorld::start()
{
	setFramebufferSize(frameBufferWidth, frameBufferHeight);

	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
		for (const auto& gameObject : it->second) {
			gameObject->start();
		}
	}

	for (auto it = gameSystems.begin(); it != gameSystems.end(); it++) {
		it->second->start();
	}
}

void GameWorld::update(double seconds)
{
	if (stop || finish) return;
	double totalTime = seconds;
	while (totalTime > 0) {
		if (totalTime > maxTimeStep) seconds = maxTimeStep;
		else seconds = totalTime;
		auto particelSystem = getGameSystem<ParticleSystem>("particle");
		if (particelSystem != nullptr) particelSystem->update(seconds);
		auto characterControllerSystem = getGameSystem<CharacterControllerSystem>("characterController");
		if (characterControllerSystem != nullptr) characterControllerSystem->update(seconds);
		auto physicsSystem = getGameSystem<PhysicsSystem>("physics");
		if (physicsSystem != nullptr) physicsSystem->update(seconds);
		auto tickSystem = getGameSystem<TickSystem>("tick");
		if (tickSystem != nullptr) tickSystem->update(seconds);
		auto collisionSystem = getGameSystem<CollisionSystem>("collision");
		if (collisionSystem != nullptr) collisionSystem->update(seconds);
		auto cameraSystem = getGameSystem<CameraSystem>("camera");
		if (cameraSystem != nullptr) cameraSystem->update(seconds);
		updateGameObjects();
		updateTransform();
		totalTime -= seconds;
	}
}

void GameWorld::draw()
{
	auto drawSystem = getGameSystem<DrawSystem>("draw");
	if (drawSystem != nullptr) drawSystem->draw();
	auto particelSystem = getGameSystem<ParticleSystem>("particle");
	if (particelSystem != nullptr) particelSystem->draw();
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
	setFramebufferSize(width, height);
}

void GameWorld::setFramebufferSize()
{
	Global::graphics.setFramebufferSize(glm::ivec2(frameBufferWidth, frameBufferHeight));
}

void GameWorld::setFramebufferSize(int width, int height)
{
	frameBufferWidth = width;
	frameBufferHeight = height;
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
	gameObject->setGameWorld(shared_from_this());
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

std::shared_ptr<GameObject> GameWorld::getGameObject(std::string tag)
{
	if (gameObjects.find(tag) == gameObjects.end()) return nullptr;
	return gameObjects[tag][0];
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
	return screen.lock();
}

std::shared_ptr<AABB> GameWorld::getAABB()
{
	return aabb;
}

void GameWorld::updateGameObjects()
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
		auto& objects = iter->second;
		for (int i = objects.size() - 1; i >= 0; i--)
			if (!objects[i]->getActiveStatus())
				objects.erase(objects.begin() + i);
	}
}

void GameWorld::updateTransform()
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
		auto& objects = iter->second;
		for (int i = 0; i < objects.size(); i++)
			objects[i]->getComponent<TransformComponent>("transform")->updateLastPos();
	}
}