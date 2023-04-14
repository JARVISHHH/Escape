#include "navmesh.h"

#include <unordered_map>

#include <Engine/Game/ai/navmesh/navmeshedge.h>
#include <Engine/Game/ai/navmesh/navmeshnode.h>
#include <Graphics/global.h>

NavMesh::NavMesh(std::string filePath) 
	: filePath(filePath)
{
	
}

void NavMesh::bake()
{
	auto mesh = Global::graphics.getNavmeshData(filePath);
	auto& positions = mesh.first;
	auto& faces = mesh.second;

	vertexPositions = positions;

	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<NavMeshEdge>>> edgesMap;
	for (const auto& face : faces) {
		auto node = std::make_shared<NavMeshNode>(shared_from_this(), face);
		nodes.push_back(node);
		int v1, v2;
		for (int i = 0; i < 3; i++) {
			v1 = face[i], v2 = face[(i + 1) % 3];
			if (v1 > v2) std::swap(v1, v2);
			// Get current edge
			std::shared_ptr<NavMeshEdge> edge;
			if (edgesMap[v1].find(v2) == edgesMap[v1].end()) {
				edge = std::make_shared<NavMeshEdge>(v1, v2);
				edges.push_back(edge);
				edgesMap[v1][v2] = edge;
			}
			edge = edgesMap[v1][v2];
			// Update graph
			for (const auto& otherNode : edge->connectedNodes) {
				otherNode->connectedNodes.push_back(node);
				node->connectedNodes.push_back(otherNode);
			}
			edge->connectedNodes.push_back(node);
			node->connectedEdges.push_back(edge);
		}
	}
	for (const auto& edge : edges) {
		if (edge->connectedNodes.size() == 1)
			edge->isInterior = false;
	}

	std::cout << "Baked NavMesh" << std::endl;
}