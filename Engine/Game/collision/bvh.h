#pragma once

#include "node.h"

class BVH {
public:
	BVH(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	std::shared_ptr<Node> buildRoot();
	void splitNode(std::shared_ptr<Node> parent, std::vector<std::shared_ptr<AABB>> curAABBs);
	void buildTree();

private:
	std::shared_ptr<Node> root;
	std::vector<std::shared_ptr<AABB>> aabbs;
};