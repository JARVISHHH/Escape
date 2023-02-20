#pragma once

#include "Engine/Game/GameComponent.h"

class CharacterJumpComponent : public GameComponent
{
public:
	CharacterJumpComponent();

	void update(double seconds);

private:
	int jumpKey = GLFW_KEY_SPACE;
	float gravity = 10;
	float fallSpeed = 0, jumpSpeed = 5;
};