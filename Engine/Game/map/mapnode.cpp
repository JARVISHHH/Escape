#include "mapnode.h"

MapNode::MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb)
	:map(map), aabb(aabb)
{
}

MapNode::MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint)
	:map(map), aabb(std::make_shared<AABB>(maxPoint, minPoint))
{
}

MapNode::~MapNode()
{
	//std::cout << "map node delete" << std::endl;
}

bool MapNode::split(glm::vec3 minimumSize, float margine, int depth)
{
	this->margine = margine;

	if (depth >= getMap()->getMaxDepth()) {
		isLeaf = true;
		return false;
	}

	//std::cout << "split" << std::endl;

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

	leftChild = std::make_shared<MapNode>(getMap(), leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(getMap(), rightMaxPoint, rightMinPoint);

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

	leftChild = std::make_shared<MapNode>(getMap(), leftMaxPoint, leftMinPoint);
	rightChild = std::make_shared<MapNode>(getMap(), rightMaxPoint, rightMinPoint);

	return true;
}

void MapNode::assign(glm::vec3 minimumSize)
{
	if (leftChild != nullptr) leftChild->assign(minimumSize);
	if (rightChild != nullptr) rightChild->assign(minimumSize);
	if (isLeaf) {
		std::vector<int> indices = { 0, 2 };
		
		glm::vec4 maxPoint = aabb->getMaxPoint(), minPoint = aabb->getMinPoint();
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

void MapNode::getLeaves(std::vector<std::shared_ptr<MapNode>>& leaves)
{
	if (leftChild != nullptr) leftChild->getLeaves(leaves);
	if (rightChild != nullptr) rightChild->getLeaves(leaves);
	if (leftChild == nullptr && rightChild == nullptr) leaves.push_back(shared_from_this());
}

std::shared_ptr<MapNode> MapNode::connect()
{
	if (leftChild == nullptr && rightChild == nullptr) return shared_from_this();

	std::shared_ptr<MapNode> left = nullptr, right = nullptr;
	left = leftChild->connect();
	right = rightChild->connect();
	std::shared_ptr<MapNode> next = nullptr;
	// up down split
	if (leftChild->aabb->getMinPoint()[0] - leftChild->margine != rightChild->aabb->getMaxPoint()[0] + rightChild->margine)
		next = rightChild->findTopRight();
	else next = rightChild->findBottomLeft();
	if(left->room->getCenter()[0] <= next->room->getCenter()[0]) map.lock()->addConnector({left, next});
	else map.lock()->addConnector({ next, left });

	return right;
}

glm::vec3 MapNode::intersect(glm::vec3 origin, glm::vec3 direction)
{
	direction = glm::normalize(direction);
	float res = -1;
	for (auto i : { 0, 2 }) {
		if (direction[i] == 0) continue;
		auto t1 = (room->getMaxPoint()[i] - origin[i]) / direction[i];
		auto t2 = (room->getMinPoint()[i] - origin[i]) / direction[i];
		if (t1 >= 0) res = res >= 0 ? std::min(res, t1) : t1;
		if (t2 >= 0) res = res >= 0 ? std::min(res, t2) : t2;
	}
	return origin + res * direction;
}

std::shared_ptr<Map> MapNode::getMap()
{
	return map.lock();
}

std::shared_ptr<AABB> MapNode::getAABB()
{
	return aabb;
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

std::shared_ptr<MapNode> MapNode::findBottomRight()
{
	if (rightChild == nullptr) return shared_from_this();
	else return rightChild->findBottomRight();
}

std::shared_ptr<MapNode> MapNode::findBottomLeft()
{
	if (leftChild == nullptr && rightChild == nullptr) return shared_from_this();
	// up down split
	if (leftChild->aabb->getMinPoint()[0] - leftChild->margine != rightChild->aabb->getMaxPoint()[0] + rightChild->margine) return rightChild->findBottomLeft();
	else return leftChild->findBottomLeft();
}

std::shared_ptr<MapNode> MapNode::findTopRight()
{
	//std::cout << leftChild << " " << rightChild << std::endl;
	if (leftChild == nullptr && rightChild == nullptr) return shared_from_this();
	// up down split
	if (leftChild->aabb->getMinPoint()[0] - leftChild->margine != rightChild->aabb->getMaxPoint()[0] + rightChild->margine) return leftChild->findTopRight();
	else return rightChild->findTopRight();
}

std::shared_ptr<MapNode> MapNode::findTopLeft()
{
	if (leftChild == nullptr) return shared_from_this();
	else return leftChild->findTopLeft();
}