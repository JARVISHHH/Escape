#pragma once

#include "navmesh.h"

class NavMeshNode {
public:
	NavMeshNode(std::shared_ptr<NavMesh> navMesh, glm::ivec3 face);

	std::shared_ptr<NavMesh> navMesh;
	glm::ivec3 positionIndex;
	glm::vec3 normal;
	glm::vec3 center;
	std::vector<std::shared_ptr<NavMeshEdge>> connectedEdges;
	std::vector<std::shared_ptr<NavMeshNode>> connectedNodes;
};