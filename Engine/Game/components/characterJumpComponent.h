#pragma once

#include "Engine/Game/gameComponent.h"

class CharacterJumpComponent : public GameComponent
{
public:
	CharacterJumpComponent();

	void update(double seconds) override;

private:
	int jumpKey = GLFW_KEY_SPACE;
	float gravity = 10;
	float jumpSpeed = 6;

	bool finishJump = true;
};