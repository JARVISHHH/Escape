#pragma once

#include "collisionComponent.h"
#include <Engine/Game/collision/ray.h>
#include <Engine/Game/collision/collisionInfo.h>

class CylinderComponent : public CollisionComponent {
public:
	glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) override;
	glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) override;

	glm::mat4x4 getTransformMatrix() override;


protected:

};