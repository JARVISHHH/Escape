#pragma once

#include "collisionComponent.h"
#include <Engine/Game/collision/ray.h>
#include <Engine/Game/collision/collisionInfo.h>

class CylinderComponent : public CollisionComponent {
public:
	glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) override;
	glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) override;

	glm::mat4x4 getInverseTransformMatrix(bool curPos = true, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) override;
	std::shared_ptr<AABB> getAABB() override;
	std::shared_ptr<AABB> getAABB(std::shared_ptr<Ray> ray) override;
	void updateOnGround() override;

protected:

};