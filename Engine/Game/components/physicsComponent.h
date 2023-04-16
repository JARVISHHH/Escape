#pragma once

#include "Engine/Game/GameComponent.h"

class PhysicsComponent : public GameComponent {
public:
	PhysicsComponent();

	void update(double seconds) override;

	float getGravity();
	glm::vec3 getVelocity();
	void applyVelocity(glm::vec3 plusVelocity);
	void setVelocity(glm::vec3 velocity);

protected:
	float gravity = 10;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
};