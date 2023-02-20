#pragma once

#include "Engine/Game/gameObject.h"

class GameComponent
{
public:
	GameComponent();

protected:
	std::shared_ptr<GameObject> gameObject;
	std::string tag;
};