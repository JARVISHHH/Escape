#pragma once

#include "Engine/Game/gameSystem.h"

class PhysicsSystem : public GameSystem {
public:
	PhysicsSystem();
	PhysicsSystem(std::shared_ptr<GameWorld> gameWorld);

	void update(double seconds);
	void addGameObject(std::shared_ptr<GameObject> gameObject);
	void updateGameObjects();

private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<GameObject>> waitingList;
};