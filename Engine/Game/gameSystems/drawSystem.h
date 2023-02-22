#pragma once

#include "Engine/Game/gameSystem.h"

class DrawSystem : public GameSystem {
public:
	DrawSystem();

	void draw();

	bool addGameObject(std::shared_ptr<GameObject> gameObject);

	void updateGameObjects();

protected:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};
