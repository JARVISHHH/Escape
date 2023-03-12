#pragma once

#include "collisionComponent.h"
#include <Engine/Game/collision/ray.h>
#include <Engine/Game/collision/collisionInfo.h>

class CylinderComponent : public CollisionComponent {
public:
	glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) override;
	glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) override;

	std::shared_ptr<CollisionInfo> getEnvironmentClosestCollision(std::shared_ptr<Ray> ray) override;
	std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> ellipsoidTriangleCollision() override;


protected:

};