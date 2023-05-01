#pragma once

#include <Engine/Game/collision/aabb.h>
#include "map.h"

class MapNode {
public:
	MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb);
	MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint);
	~MapNode();

	bool split(glm::vec3 minimumSize, float margine, int depth);
	bool splitX(glm::vec3 minimumSize, float margine);
	bool splitZ(glm::vec3 minimumSize, float margine);

	void assign(glm::vec3 minimumSize);

	std::shared_ptr<Map> getMap();
	void printNode();

	std::shared_ptr<AABB> room = nullptr;
	std::shared_ptr<MapNode> leftChild = nullptr, rightChild = nullptr;

protected:

private:
	std::weak_ptr<Map> map;
	std::shared_ptr<AABB> aabb;

	bool isLeaf = false;

};