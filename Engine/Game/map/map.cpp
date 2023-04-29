#include "map.h"
#include "mapnode.h"

Map::Map(std::shared_ptr<GameWorld> gameWorld, int maxDepth)
	:gameWorld(gameWorld), aabb(gameWorld->getAABB()), maxDepth(maxDepth)
{
}

void Map::generateMap()
{
	mapRoot = std::make_shared<MapNode>(shared_from_this(), aabb);
	mapRoot->split(minimumSize, 0);
}

void Map::printMap()
{
	if (mapRoot != nullptr) {
		mapRoot->printNode();
	}
}