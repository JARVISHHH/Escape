#pragma once

#include <Game/components.h>

class ShootComponent : public GameComponent {
public:
	ShootComponent(std::string targetTag, bool stop);

	void start() override;
	void update(double seconds);

	void setStop(bool stop);
	void shoot();

private:
	std::string targetTag;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<TransformComponent> targetTransform;

	bool stop = true;
	float shootInterval = 2;
	float leftInterval = 2;
};