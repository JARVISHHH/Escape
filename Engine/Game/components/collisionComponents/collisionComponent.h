#pragma once

#include "Engine/Game/gameComponent.h"

class CylinderComponent;

class CollisionComponent : public std::enable_shared_from_this<CollisionComponent>, public GameComponent {
public:
	CollisionComponent();

	virtual glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) = 0;
	virtual glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) = 0;

	virtual std::shared_ptr<CollisionInfo> getEnvironmentClosestCollision(std::shared_ptr<Ray> ray) = 0;
	virtual std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> ellipsoidTriangleCollision() = 0;
};