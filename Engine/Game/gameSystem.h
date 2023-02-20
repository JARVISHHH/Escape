#pragma once

#include "gameWorld.h"
#include <string>

class GameSystem
{
public:
	GameSystem(std::string tag);

	std::string getTag() const;
	void setGameWorld(std::shared_ptr<GameWorld> gameWorld);

protected:
	std::shared_ptr<GameWorld> gameWorld;
	const std::string tag;
};