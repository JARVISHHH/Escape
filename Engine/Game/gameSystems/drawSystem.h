#pragma once

#include "Engine/Game/gameSystem.h"

class DrawSystem : public GameSystem {
public:
	DrawSystem();
	DrawSystem(std::shared_ptr<GameWorld> gameWorld);

	void draw();

	bool addGameObject(std::shared_ptr<GameObject> gameObject);

	void updateGameObjects();

protected:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};
