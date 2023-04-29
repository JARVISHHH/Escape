#include "mapnode.h"

MapNode::MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb)
	:map(map), aabb(aabb)
{
}

MapNode::MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint)
	:map(map), aabb(std::make_shared<AABB>(maxPoint, minPoint))
{
}

bool MapNode::split(glm::vec3 minimumSize, float margine, int depth)
{
	if (depth >= map->getMaxDepth()) {
		isLeaf = true;
		return false;
	}

	std::cout << "split" << std::endl;

	float x = aabb->getMaxPoint()[0] - aabb->getMinPoint()[0];
	float z = aabb->getMaxPoint()[2] - aabb->getMinPoint()[2];

	bool res;

	if (x >= z) res = splitX(minimumSize, margine);
	else res = splitZ(minimumSize, margine);

	if (res) {
		leftChild->split(minimumSize, margine, depth + 1);
		rightChild->split(minimumSize, margine, depth + 1);
	}
	else {
		isLeaf = true;
	}

	return res;
}

bool MapNode::splitX(glm::vec3 minimumSize, float margine)
{
	float x = aabb->getMaxPoint()[0] - aabb->getMinPoint()[0];
	if (x <= 2 * minimumSize[0] + 2 * margine) return false;

	float splitSize = (float)rand() / RAND_MAX * (x - 2 * minimumSize[0] - 2 * margine) + minimumSize[0] + margine;

	glm::vec4 leftMaxPoint = aabb->getMaxPoint(), rightMinPoint = aabb->getMinPoint();
	glm::vec4 leftMinPoint = aabb->getMinPoint(), rightMaxPoint = aabb->getMaxPoint();
	leftMinPoint[0] = leftMaxPoint[0] - splitSize + margine, rightMaxPoint[0] = leftMaxPoint[0] - splitSize - margine;

	leftChild = std::make_shared<MapNode>(map, leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(map, rightMaxPoint, rightMinPoint);

	return true;
}

bool MapNode::splitZ(glm::vec3 minimumSize, float margine)
{
	float z = aabb->getMaxPoint()[2] - aabb->getMinPoint()[2];
	if (z <= 2 * minimumSize[2] + 2 * margine) return false;

	float splitSize = (float)rand() / RAND_MAX * (z - 2 * minimumSize[2] - 2 * margine) + minimumSize[2] + margine;

	glm::vec4 leftMaxPoint = aabb->getMaxPoint(), rightMinPoint = aabb->getMinPoint();
	glm::vec4 leftMinPoint = aabb->getMinPoint(), rightMaxPoint = aabb->getMaxPoint();
	leftMinPoint[2] = leftMaxPoint[2] - splitSize + margine, rightMaxPoint[2] = leftMaxPoint[2] - splitSize - margine;

	leftChild = std::make_shared<MapNode>(map, leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(map, rightMaxPoint, rightMinPoint);

	return true;
}

void MapNode::assign(glm::vec3 minimumSize)
{
	if (leftChild != nullptr) leftChild->assign(minimumSize);
	if (rightChild != nullptr) rightChild->assign(minimumSize);
	if (isLeaf) {
		std::vector<int> indices = { 0, 2 };
		glm::vec4 maxPoint(0, 0, 0, 1), minPoint(0, 0, 0, 1);
		for (auto i : indices) {
			auto total = aabb->getMaxPoint()[i] - aabb->getMinPoint()[i];
			auto usable = total;
			auto roomSize = std::max((float)rand() / RAND_MAX * (usable / 3) + usable * 2 / 3, minimumSize[i]);
			auto pos = aabb->getMinPoint()[i] + (float)rand() / RAND_MAX * (total - roomSize);
			minPoint[i] = pos, maxPoint[i] = minPoint[i] + roomSize;
		}
		room = std::make_shared<AABB>(maxPoint, minPoint);
	}
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