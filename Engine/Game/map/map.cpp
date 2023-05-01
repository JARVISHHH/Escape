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
}

void Map::printMap()
{
	if (mapRoot != nullptr) {
		mapRoot->printNode();
	}
}