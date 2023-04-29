#pragma once

#include <memory>
#include <Engine/Game/gameWorld.h>

class MapNode;

class Map: public std::enable_shared_from_this<Map> {
public:
	Map(std::shared_ptr<GameWorld> gameWorld, int maxDepth = 3);

	void generateMap();
	int getMaxDepth() { return maxDepth; }

	void printMap();

protected:


private:
	std::shared_ptr<GameWorld> gameWorld;

	int maxDepth = 3;
	std::shared_ptr<AABB> aabb;
	glm::vec3 minimumSize = glm::vec3(3, 5, 3);

	std::shared_ptr<MapNode> mapRoot;

};