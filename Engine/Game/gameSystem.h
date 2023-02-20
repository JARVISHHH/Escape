#pragma once

#include "gameWorld.h"
#include <string>

class GameSystem
{
public:
	GameSystem(std::string tag);

	std::string getTag() const;
	bool addGameObject(std::shared_ptr<GameObject> gameObject);

protected:
	const std::string tag;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};