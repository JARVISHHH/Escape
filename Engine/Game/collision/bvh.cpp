#include "bvh.h"

#include <algorithm>

BVH::BVH(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	aabbs.clear();
	aabbs.resize(environmentComponents.size());
	for (const auto& environmentComponent : environmentComponents)
		aabbs.push_back(environmentComponent->getBoundingBox());
}

std::shared_ptr<Node> BVH::buildRoot()
{
	if (aabbs.size() == 0) return nullptr;

	std::shared_ptr<Node> root = std::make_shared<Node>();
	glm::vec4 maxPoint = aabbs[0]->getMaxPoint(), minPoint = aabbs[0]->getMinPoint();
	for (const auto& aabb : aabbs) {
		for (int i = 0; i < 3; i++) {
			maxPoint[i] = std::max(maxPoint[i], aabb->getMaxPoint()[i]);
			minPoint[i] = std::min(minPoint[i], aabb->getMinPoint()[i]);
		}
	}
	root->boundingBox = std::make_shared<AABB>(maxPoint, minPoint);

	return root;
}

void BVH::splitNode(std::shared_ptr<Node> parent, std::vector<std::shared_ptr<AABB>> curAABBs)
{
	if (curAABBs.size() < 3) {
		parent->aabbs = curAABBs;
		return;
	}

	float differences[3];
	for (int i = 0; i < 3; i++) differences[i] = parent->boundingBox->getMaxPoint()[i] - parent->boundingBox->getMinPoint()[i];
	if (differences[0] >= differences[1] && differences[0] >= differences[2]) sort(curAABBs.begin(), curAABBs.end(), AABB::compareX);
	else if (differences[1] >= differences[0] && differences[1] >= differences[2]) sort(curAABBs.begin(), curAABBs.end(), AABB::compareY);
	else /*if (differences[2] >= differences[0] && differences[2] >= differences[1]) */sort(curAABBs.begin(), curAABBs.end(), AABB::compareZ);

	std::vector<std::shared_ptr<AABB>> leftAABBs(curAABBs.begin(), curAABBs.begin() + curAABBs.size() / 2), rightAABBs(curAABBs.begin() + curAABBs.size() / 2, curAABBs.end());
	glm::vec4 maxPoint, minPoint;

	// Build left node
	maxPoint = leftAABBs[0]->getMaxPoint(), minPoint = rightAABBs[0]->getMinPoint();
	for (const auto& aabb : leftAABBs) {
		for (int i = 0; i < 3; i++) {
			maxPoint[i] = std::max(maxPoint[i], aabb->getMaxPoint()[i]);
			minPoint[i] = std::min(minPoint[i], aabb->getMinPoint()[i]);
		}
	}
	std::shared_ptr<AABB> leftBoundingBox = std::make_shared<AABB>(maxPoint, minPoint);

	// Build right node
	maxPoint = rightAABBs[0]->getMaxPoint(), minPoint = rightAABBs[0]->getMinPoint();
	for (const auto& aabb : rightAABBs) {
		for (int i = 0; i < 3; i++) {
			maxPoint[i] = std::max(maxPoint[i], aabb->getMaxPoint()[i]);
			minPoint[i] = std::min(minPoint[i], aabb->getMinPoint()[i]);
		}
	}
	std::shared_ptr<AABB> rightBoundingBox = std::make_shared<AABB>(maxPoint, minPoint);

	auto left = std::make_shared<Node>(leftBoundingBox), right = std::make_shared<Node>(rightBoundingBox);
	splitNode(left, leftAABBs);
	splitNode(right, rightAABBs);
	parent->left = left;
	parent->right = right;
}

void BVH::buildTree()
{
	root = buildRoot();
	splitNode(root, aabbs);
	std::cout << "Build BVH" << std::endl;
}
