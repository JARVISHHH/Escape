#pragma once

#include "gameWorld.h"

class GameSystem
{
public:


protected:
	std::string tag;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};