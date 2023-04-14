#include "navmeshnode.h"

NavMeshNode::NavMeshNode(std::shared_ptr<NavMesh> navMesh, glm::ivec3 face)
	: navMesh(navMesh), positionIndex(face)
{
	glm::vec3 v1, v2, v3;
	v1 = navMesh->vertexPositions[face[0]];
	v2 = navMesh->vertexPositions[face[1]];
	v3 = navMesh->vertexPositions[face[2]];
	normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
	center = (v1 + v2 + v3) / 3.0f;
}