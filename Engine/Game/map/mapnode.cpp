#include "mapnode.h"

MapNode::MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb)
	:map(map), aabb(aabb)
{
}

MapNode::MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint)
	:map(map), aabb(std::make_shared<AABB>(maxPoint, minPoint))
{
}

bool MapNode::split(glm::vec3 minimumSize, int depth)
{
	if (depth >= map->getMaxDepth()) return false;

	float x = aabb->getMaxPoint()[0] - aabb->getMinPoint()[0];
	float z = aabb->getMaxPoint()[2] - aabb->getMinPoint()[2];

	bool res;

	if (x >= z) res = splitX(minimumSize);
	else res = splitZ(minimumSize);

	if (res) {
		leftChild->split(minimumSize, depth + 1);
		rightChild->split(minimumSize, depth + 1);
	}

	return res;
}

bool MapNode::splitX(glm::vec3 minimumSize)
{
	float x = aabb->getMaxPoint()[0] - aabb->getMinPoint()[0];
	if (x <= 2 * minimumSize[0]) return false;

	float splitSize = (float)rand() / RAND_MAX * (x - 2 * minimumSize[0]) + minimumSize[0];

	glm::vec4 leftMaxPoint = aabb->getMaxPoint(), rightMinPoint = aabb->getMinPoint();
	glm::vec4 leftMinPoint = aabb->getMinPoint(), rightMaxPoint = aabb->getMaxPoint();
	leftMinPoint[0] = leftMaxPoint[0] - splitSize, rightMaxPoint[0] = leftMaxPoint[0] - splitSize;

	leftChild = std::make_shared<MapNode>(map, leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(map, rightMaxPoint, rightMinPoint);

	return true;
}

bool MapNode::splitZ(glm::vec3 minimumSize)
{
	float z = aabb->getMaxPoint()[2] - aabb->getMinPoint()[2];
	if (z <= 2 * minimumSize[2]) return false;

	float splitSize = (float)rand() / RAND_MAX * (z - 2 * minimumSize[2]) + minimumSize[2];

	glm::vec4 leftMaxPoint = aabb->getMaxPoint(), rightMinPoint = aabb->getMinPoint();
	glm::vec4 leftMinPoint = aabb->getMinPoint(), rightMaxPoint = aabb->getMaxPoint();
	leftMinPoint[2] = leftMaxPoint[2] - splitSize, rightMaxPoint[2] = leftMaxPoint[2] - splitSize;

	leftChild = std::make_shared<MapNode>(map, leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(map, rightMaxPoint, rightMinPoint);

	return true;
}

void MapNode::printNode()
{
	std::cout << "print node" << std::endl;
	aabb->printMaxPoint();
	aabb->printMinPoint();
	if (leftChild == nullptr) std::cout << "left null" << std::endl;
	if (rightChild == nullptr) std::cout << "right null" << std::endl;
	if (leftChild != nullptr) leftChild->printNode();
	if (rightChild != nullptr) rightChild->printNode();
}