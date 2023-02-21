#pragma once

#include "Engine/Game/gameComponent.h"

class CylinderComponent;

class CollisionComponent : public GameComponent {
public:
	CollisionComponent();

	virtual bool checkCollision(std::shared_ptr<CollisionComponent> component) = 0;
	virtual bool checkCollision(std::shared_ptr<CylinderComponent> component) = 0;
};