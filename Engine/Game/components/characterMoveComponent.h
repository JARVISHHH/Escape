#pragma once

#include "Engine/Game/GameComponent.h"

class CharacterMoveComponent : public GameComponent
{
public:
	CharacterMoveComponent();

	void update(double seconds);

private:
	int moveKeys[4] = { GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D };
	float speed = 1.5;
};
