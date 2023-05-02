#pragma once

#include <memory>
#include <Engine/Game/gameWorld.h>

class MapNode;

class Map: public std::enable_shared_from_this<Map> {
public:
	Map(std::shared_ptr<GameWorld> gameWorld, int maxDepth = 3);
	~Map();

	void generateMap();
	int getMaxDepth() { return maxDepth; }

	void addConnector(std::pair<glm::vec3, glm::vec3> connector);
	std::vector<std::pair<glm::vec3, glm::vec3>>& getConnectors();

	void printMap();

	std::shared_ptr<MapNode> mapRoot;

protected:


private:
	std::weak_ptr<GameWorld> gameWorld;

	int maxDepth = 3;
	std::shared_ptr<AABB> aabb;
	glm::vec3 minimumSize = glm::vec3(10, 5, 10);
	float margine = 0.5;
	std::vector<std::pair<glm::vec3, glm::vec3>> connectors;

	void splitSpace();
	void assignRoom();
	void connect();
};