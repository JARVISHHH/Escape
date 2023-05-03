#include "navmeshedge.h"
#include <iostream>

NavMeshEdge::NavMeshEdge(int vertexIndex1, int vertexIndex2)
	: vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2)
{
}

NavMeshEdge::~NavMeshEdge()
{
	//std::cout << "navmesh edge delete" << std::endl;
}