#pragma once

#include <Engine/Game/collision/aabb.h>
#include "map.h"

class MapNode {
public:
	MapNode(std::shared_ptr<Map> map, std::shared_ptr<AABB> aabb);
	MapNode(std::shared_ptr<Map> map, glm::vec4 maxPoint, glm::vec4 minPoint);

	bool split(glm::vec3 minimumSize, int depth);
	bool splitX(glm::vec3 minimumSize);
	bool splitZ(glm::vec3 minimumSize);

	void printNode();

protected:

private:
	std::shared_ptr<Map> map;
	std::shared_ptr<AABB> aabb;

	std::shared_ptr<MapNode> leftChild = nullptr, rightChild = nullptr;

};