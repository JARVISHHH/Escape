#pragma once

#include "Engine/Game/components/physicsComponent.h"

class MovingBoard : public PhysicsComponent {
public:
	MovingBoard(float velocity, glm::vec3 startPos, glm::vec3 endPos);

	void update(double seconds) override;

private:
	glm::vec3 startPos, endPos;
};