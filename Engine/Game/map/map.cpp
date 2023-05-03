#include "map.h"
#include "mapnode.h"

Map::Map(std::shared_ptr<AABB> aabb, glm::vec3 minimumSize, float margine, int maxDepth)
	:aabb(aabb), minimumSize(minimumSize), margine(margine), maxDepth(maxDepth)
{
}

Map::~Map()
{
	//std::cout << "map delete" << std::endl;
}

void Map::generateMap()
{
	mapRoot = std::make_shared<MapNode>(shared_from_this(), aabb);
	splitSpace();
	assignRoom();
	connect();
}

void Map::splitSpace()
{
	mapRoot->split(minimumSize, margine, 0);
}

void Map::assignRoom()
{
	mapRoot->assign(minimumSize);
}

void Map::connect()
{
	if (mapRoot != nullptr) {
		connectors.clear();
		//std::shared_ptr<MapNode> left = nullptr, right = nullptr;
		//if (mapRoot->leftChild != nullptr) left = mapRoot->leftChild->connect(nullptr);
		//if (mapRoot->rightChild != nullptr) {
		//	mapRoot->rightChild->connect(nullptr);
		//	right = mapRoot->rightChild->findBottomLeft();
		//}
		//if (left != nullptr && right != nullptr) left->connect(right);
		mapRoot->connect();
	}
}

void Map::addConnector(std::pair<std::shared_ptr<MapNode>, std::shared_ptr<MapNode>> connector)
{
	connectors.push_back(connector);
}

std::vector<std::pair<std::shared_ptr<MapNode>, std::shared_ptr<MapNode>>>& Map::getConnectors()
{
	return connectors;
}

void Map::printMap()
{
	if (mapRoot != nullptr) {
		mapRoot->printNode();
	}
}