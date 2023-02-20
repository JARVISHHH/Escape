#pragma once

#include <memory>

#include "Engine/Game/gameSystem.h"

class CharacterControllerSystem : public GameSystem
{
public:
	CharacterControllerSystem();

	void update(double seconds);

	bool setCharatcer(std::shared_ptr<GameObject> character);

private:
	std::shared_ptr<GameWorld> gameWorld;
	std::shared_ptr<GameObject> character;
};