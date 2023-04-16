#pragma once

#include "navmeshnode.h"

class NavMeshEdge {
public:
	NavMeshEdge(int vertexIndex1, int vertexIndex12);

	int vertexIndex1, vertexIndex2;
	std::vector<std::shared_ptr<NavMeshNode>> connectedNodes;

	bool isInterior = true;
};