#pragma once

#include <memory>

#include "aabb.h"

class BVHNode {
public:
	BVHNode() {};
	BVHNode(std::shared_ptr<AABB> aabb) { boundingBox = aabb; }
	//~BVHNode() { std::cout << "BVHNode delete" << std::endl; }

	std::shared_ptr<AABB> boundingBox;
	std::shared_ptr<BVHNode> left = nullptr;
	std::shared_ptr<BVHNode> right = nullptr;
	std::vector<std::shared_ptr<AABB>> aabbs;

	std::shared_ptr<CollisionInfo> getCollision(glm::mat4x4 transformMatrix, std::shared_ptr<AABB> movingAABB, std::shared_ptr<Ray> sphereSpaceRay);
};