#pragma once

#include <memory>

#include "aabb.h"

class Node {
public:
	Node() {};
	Node(std::shared_ptr<AABB> aabb) { boundingBox = aabb; }

	std::shared_ptr<AABB> boundingBox;
	std::shared_ptr<Node> left = nullptr;
	std::shared_ptr<Node> right = nullptr;
	std::vector<std::shared_ptr<AABB>> aabbs;

	std::shared_ptr<CollisionInfo> getCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray, std::shared_ptr<Ray> sphereSpaceRay);
};