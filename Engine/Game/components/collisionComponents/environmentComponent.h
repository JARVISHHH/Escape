#pragma once

#include <vector>
#include <memory>
#include <Engine/Mesh/triangle.h>

#include "Engine/Game/gameComponent.h"

class AABB;

class EnvironmentComponent : public GameComponent, public std::enable_shared_from_this<EnvironmentComponent> {
public:
	EnvironmentComponent(std::vector<std::shared_ptr<Triangle>> triangles);
	~EnvironmentComponent();

	void buildBoundingBox();
	std::shared_ptr<CollisionInfo> getClosestCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray);
	const std::vector<std::shared_ptr<Triangle>>& getTriangles();
	std::vector<std::shared_ptr<AABB>>& getBoundingBox() { return aabbs; }

private:
	std::vector<std::shared_ptr<Triangle>> triangles;
	std::vector<std::shared_ptr<AABB>> aabbs;
};