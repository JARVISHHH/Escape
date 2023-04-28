#pragma once

#include <Game/components.h>

class MovingComponent : public GameComponent {
public:
	MovingComponent(glm::vec3 direction, float speed = 3);

	void start() override;
	void update(double seconds) override;

private:
	glm::vec3 direction;
	float speed;

	std::shared_ptr<TransformComponent> transform;
};
