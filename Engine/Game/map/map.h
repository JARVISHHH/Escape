#pragma once

#include <memory>
#include <Engine/Game/gameWorld.h>

class MapNode;

class Map: public std::enable_shared_from_this<Map> {
public:
	Map(std::shared_ptr<AABB> aabb, glm::vec3 minimumSize = glm::vec3(7.5, 5, 7.5), float margine = 0.5, int maxDepth = 5);
	~Map();

	void generateMap(bool generateRoom = true, bool connectRoom = true);
	int getMaxDepth() { return maxDepth; }

	void addConnector(std::pair<std::shared_ptr<MapNode>, std::shared_ptr<MapNode>> connector);
	std::vector<std::pair<std::shared_ptr<MapNode>, std::shared_ptr<MapNode>>>& getConnectors();
	std::vector<std::shared_ptr<MapNode>> findConnectors(std::shared_ptr<MapNode> mapNode);

	std::shared_ptr<MapNode> findNode(std::shared_ptr<GameObject> object);

	void printMap();

	std::shared_ptr<MapNode> mapRoot;
	std::vector<std::shared_ptr<MapNode>> leaves;

protected:


private:
	//std::weak_ptr<GameWorld> gameWorld;

	int maxDepth = 3;
	std::shared_ptr<AABB> aabb;
	glm::vec3 minimumSize = glm::vec3(10, 5, 10);
	float margine = 0.5;
	std::vector<std::pair<std::shared_ptr<MapNode>, std::shared_ptr<MapNode>>> connectors;

	void splitSpace();
	void assignRoom();
	void connect();
	void getLeaves();
};