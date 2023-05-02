#pragma once

#include "collisionComponent.h"
#include <Engine/Game/collision/ray.h>
#include <Engine/Game/collision/collisionInfo.h>

class CylinderComponent : public CollisionComponent {
public:
	glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) override;
	glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) override;

	glm::mat4x4 getTransformMatrix() override;
	std::shared_ptr<AABB> getAABB() override;
	std::shared_ptr<AABB> getAABB(std::shared_ptr<Ray> ray) override;
	void updateOnGround() override;

	static std::shared_ptr<AABB> getAABB(std::shared_ptr<ModelTransform> modelTransform);
	static std::shared_ptr<AABB> getAABB(std::shared_ptr<ModelTransform> modelTransform, std::shared_ptr<Ray> ray);

protected:

};