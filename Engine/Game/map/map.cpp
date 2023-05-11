#include "map.h"
#include "mapnode.h"
#include <Engine/Game/components/collisionComponents/collisionComponent.h>

Map::Map(std::shared_ptr<AABB> aabb, glm::vec3 minimumSize, float margine, int maxDepth)
	:aabb(aabb), minimumSize(minimumSize), margine(margine), maxDepth(maxDepth)
{
}

Map::~Map()
{
	//std::cout << "map delete" << std::endl;
}

void Map::generateMap(bool generateRoom, bool connectRoom)
{
	mapRoot = std::make_shared<MapNode>(shared_from_this(), aabb);
	splitSpace();
	getLeaves();
	if(generateRoom) assignRoom();
	if(generateRoom && connectRoom) connect();
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
		mapRoot->connect();
	}
}

void Map::getLeaves()
{
	if (mapRoot != nullptr) {
		mapRoot->getLeaves(leaves);
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

std::shared_ptr<MapNode> Map::findNode(std::shared_ptr<GameObject> object)
{
	for (auto& mapNode : leaves) {
		if(mapNode->getAABB()->checkCollision(object->getComponent<CollisionComponent>("collision")->getAABB()))
			return mapNode;
	}

	return nullptr;
}

void Map::printMap()
{
	if (mapRoot != nullptr) {
		mapRoot->printNode();
	}
}