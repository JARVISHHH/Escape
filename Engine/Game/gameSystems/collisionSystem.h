#pragma once

#include "Engine/Game/gameSystem.h"

class CollisionSystem : public GameSystem {
public:
	CollisionSystem();

	void update(double seconds);
	void doCollision();
	void addGameObject(std::shared_ptr<GameObject> gameObject);

protected:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};