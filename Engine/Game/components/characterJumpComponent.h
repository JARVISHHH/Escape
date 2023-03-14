#pragma once

#include "Engine/Game/gameComponent.h"

class CharacterJumpComponent : public GameComponent
{
public:
	CharacterJumpComponent();

	void update(double seconds);

private:
	int jumpKey = GLFW_KEY_SPACE;
	float gravity = 10;
	float jumpSpeed = 1;
};