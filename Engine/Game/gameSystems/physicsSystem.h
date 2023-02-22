#pragma once

#include "Engine/Game/gameSystem.h"

class PhysicsSystem : public GameSystem {
public:
	PhysicsSystem();

	void update(double seconds);
	void addGameObject(std::shared_ptr<GameObject> gameObject);

private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};