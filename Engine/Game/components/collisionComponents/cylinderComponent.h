#pragma once

#include "collisionComponent.h"

class CylinderComponent : public CollisionComponent {
public:
	CylinderComponent();

	bool checkCollision(std::shared_ptr<CollisionComponent> component);
	bool checkCollision(std::shared_ptr<CylinderComponent> component);

protected:
	std::shared_ptr<CylinderComponent> self;
};