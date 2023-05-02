#include "map.h"
#include "mapnode.h"

Map::Map(std::shared_ptr<GameWorld> gameWorld, int maxDepth)
	:gameWorld(gameWorld), aabb(gameWorld->getAABB()), maxDepth(maxDepth)
{
}

Map::~Map()
{
	std::cout << "map delete" << std::endl;
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
		/*std::shared_ptr<MapNode> left = nullptr, right = nullptr;
		if (mapRoot->leftChild != nullptr) left = mapRoot->leftChild->connect(nullptr);
		if (mapRoot->rightChild != nullptr) {
			mapRoot->rightChild->connect(nullptr);
			right = mapRoot->rightChild->findBottomLeft();
		}
		if (left != nullptr && right != nullptr) left->connect(right);*/
		mapRoot->connect();
	}
}

void Map::addConnector(std::pair<glm::vec3, glm::vec3> connector)
{
	connectors.push_back(connector);
}

std::vector<std::pair<glm::vec3, glm::vec3>>& Map::getConnectors()
{
	return connectors;
}

void Map::printMap()
{
	if (mapRoot != nullptr) {
		mapRoot->printNode();
	}
}