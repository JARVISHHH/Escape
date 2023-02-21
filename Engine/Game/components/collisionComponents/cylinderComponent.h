#pragma once

#include "collisionComponent.h"

class CylinderComponent : public CollisionComponent {
public:
	CylinderComponent();

	glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) override;
	glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) override;

protected:
	std::shared_ptr<CylinderComponent> self;
};