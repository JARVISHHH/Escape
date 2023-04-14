#include "navmeshnode.h"

NavMeshNode::NavMeshNode(std::shared_ptr<NavMesh> navMesh, glm::ivec3 face)
	: navMesh(navMesh), positionIndex(face)
{

}