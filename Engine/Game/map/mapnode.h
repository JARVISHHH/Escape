#pragma once

#include <Engine/Game/collision/aabb.h>
#include <Engine/Game/ai/navmesh/navmesh.h>
#include "map.h"

class MapNode: public std::enable_shared_from_this<MapNode> {
public:
	MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb);
	MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint);
	~MapNode();

	bool split(glm::vec3 minimumSize, float margine, int depth);
	bool splitX(glm::vec3 minimumSize, float margine);
	bool splitZ(glm::vec3 minimumSize, float margine);

	void assign(glm::vec3 minimumSize);

	std::shared_ptr<MapNode> connect();
	std::shared_ptr<MapNode> findBottomLeft();
	std::shared_ptr<MapNode> findTopRight();

	glm::vec3 intersect(glm::vec3 origin, glm::vec3 direction);

	std::shared_ptr<Map> getMap();
	std::shared_ptr<AABB> getAABB();
	void printNode();

	std::shared_ptr<AABB> room = nullptr;
	std::shared_ptr<MapNode> leftChild = nullptr, rightChild = nullptr;
	std::shared_ptr<NavMesh> navMesh;

	//std::vector<std::pair<glm::vec3, glm::vec3>> gaps;
	std::vector<glm::vec3> gapStarts;
	std::vector<glm::vec3> gapEnds;

protected:

private:
	std::weak_ptr<Map> map;
	std::shared_ptr<AABB> aabb;

	bool isLeaf = false;
	float margine;

};