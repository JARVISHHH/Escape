#pragma once

#include "bvhnode.h"

class BVH {
public:
	BVH(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);

	// Build
	std::shared_ptr<BVHNode> buildRoot();
	void splitNode(std::shared_ptr<BVHNode> parent, std::vector<std::shared_ptr<AABB>> curAABBs);
	void buildTree();

	// Collide
	std::shared_ptr<CollisionInfo> getClosestCollision(glm::mat4x4 transformMatrix, std::shared_ptr<AABB> movingAABB, std::shared_ptr<Ray> sphereSpaceRay);

private:
	std::shared_ptr<BVHNode> root;
	std::vector<std::shared_ptr<AABB>> aabbs;

	std::shared_ptr<CollisionInfo> dfs(glm::mat4x4 transformMatrix, std::shared_ptr<Ray> ray);
};