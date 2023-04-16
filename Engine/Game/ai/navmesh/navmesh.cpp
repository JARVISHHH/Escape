#include "navmesh.h"

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
				commonEdges[node][otherNode] = edge;
				commonEdges[otherNode][node] = edge;
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
	std::cout << "nodes: " << nodes.size() << std::endl;
	std::cout << "edges: " << edges.size() << std::endl;
}

std::vector<glm::vec3> NavMesh::pathFinding(glm::vec3 start, glm::vec3 end) {
	// Get start and end positions
	std::shared_ptr<NavMeshNode> startNode, endNode;
	glm::vec3 direction = { 0, -1, 0 };
	std::cout << "original start: " << start[0] << " " << start[1] << " " << start[2] << std::endl;
	std::cout << "original end: " << end[0] << " " << end[1] << " " << end[2] << std::endl;
	float tStart = rayCast(start, direction, startNode), tEnd = rayCast(end, direction, endNode);
	if (tStart < 0 || tEnd < 0) {
		if(tStart < 0) std::cerr << "Pathfinding failed: start ray cast failed!" << std::endl;
		if (tEnd < 0) std::cerr << "Pathfinding failed: end ray cast failed!" << std::endl;
		return {};
	}
	glm::vec3 startPos = start + tStart * direction, endPos = end + tEnd * direction;

	std::cout << "start: " << startPos[0] << " " << startPos[1] << " " << startPos[2] << std::endl;
	std::cout << "end: " << endPos[0] << " " << endPos[1] << " " << endPos[2] << std::endl;

	auto path = aStar(startPos, endPos, startNode, endNode);

	for (int i = 0; i < path.size(); i++) {
		std::cout << i << ": " << path[i][0] << " " << path[i][1] << " " << path[i][2] << std::endl;
	}

	return path;
}

float NavMesh::getEuclidianDistance(std::shared_ptr<NavMeshNode> node, glm::vec3 target) {
	return glm::length(node->center - target);
}

float NavMesh::getEuclidianDistance(std::shared_ptr<NavMeshEdge> edge, glm::vec3 target) {
	glm::vec3 edgeCenter = (vertexPositions[edge->vertexIndex1] - vertexPositions[edge->vertexIndex2]) / 2.0f;
	return glm::length(edgeCenter - target);
}

float NavMesh::getEuclidianDistance(std::shared_ptr<NavMeshNode> node1, std::shared_ptr<NavMeshNode> node2) {
	std::shared_ptr<NavMeshEdge> commonEdge = commonEdges[node1][node2];
	if (commonEdge == nullptr) {
		std::cerr << "Common edge searching failed!" << std::endl;
	}
	glm::vec3 edgeCenter = (vertexPositions[commonEdge->vertexIndex1] - vertexPositions[commonEdge->vertexIndex2]) / 2.0f;
	return glm::length(node1->center - node2->center);
}

float NavMesh::getEuclidianDistance(std::shared_ptr<NavMeshEdge> edge1, std::shared_ptr<NavMeshEdge> edge2) {
	glm::vec3 edgeCenter1 = (vertexPositions[edge1->vertexIndex1] - vertexPositions[edge1->vertexIndex2]) / 2.0f;
	glm::vec3 edgeCenter2 = (vertexPositions[edge2->vertexIndex1] - vertexPositions[edge2->vertexIndex2]) / 2.0f;
	return glm::length(edgeCenter1 - edgeCenter2);
}

//std::vector<std::shared_ptr<NavMeshNode>> NavMesh::aStar(glm::vec3 startPos, glm::vec3 endPos, std::shared_ptr<NavMeshNode> startNode, std::shared_ptr<NavMeshNode> endNode)
//{
//	std::unordered_map<std::shared_ptr<NavMeshNode>, float> lowestCost;
//	std::unordered_map<std::shared_ptr<NavMeshNode>, std::shared_ptr<NavMeshNode>> lastNode;
//	std::unordered_set<std::shared_ptr<NavMeshNode>> closedList;
//	std::priority_queue<std::pair<float, std::shared_ptr<NavMeshNode>>> openList;
//
//	bool getEnd = (startNode == endNode);
//	openList.push({getEuclidianDistance(startNode, startPos) + getEuclidianDistance(startNode, endPos), startNode});
//	while (!openList.empty()) {
//		auto cost = openList.top().first;
//		auto currentNode = openList.top().second;
//		openList.pop();
//		if (currentNode == endNode) {
//			getEnd = true;
//			break;
//		}
//		if (closedList.find(currentNode) != closedList.end()) continue;  // In closed list
//		closedList.insert(currentNode);
//		auto costH = getEuclidianDistance(currentNode, endPos);
//		for (const auto& connectedNode : currentNode->connectedNodes) {
//			if (closedList.find(connectedNode) != closedList.end()) continue;  // In closed list
//			float newCost = cost - costH + getEuclidianDistance(currentNode, connectedNode) + getEuclidianDistance(connectedNode, endPos);
//			if (lowestCost.find(connectedNode) == lowestCost.end() || lowestCost[connectedNode] > newCost) {
//				lowestCost[connectedNode] = newCost;
//				lastNode[connectedNode] = currentNode;
//			}
//		}
//	}
//	
//	std::vector<std::shared_ptr<NavMeshNode>> path;
//
//	if (!getEnd) {
//		std::cerr << "A* failed: heap is empty before getting to the end!" << std::endl;
//		return path;
//	}
//
//	// Get shortest path
//	auto currentNode = endNode;
//	while (currentNode != startNode) {
//		path.push_back(currentNode);
//		currentNode = lastNode[currentNode];
//	}
//	path.push_back(startNode);
//	std::reverse(path.begin(), path.end());
//
//	return path;
//}

std::vector<glm::vec3> NavMesh::aStar(glm::vec3 startPos, glm::vec3 endPos, std::shared_ptr<NavMeshNode> startNode, std::shared_ptr<NavMeshNode> endNode)
{
	std::unordered_map<std::shared_ptr<NavMeshEdge>, float> lowestCost;
	std::unordered_map<std::shared_ptr<NavMeshEdge>, std::shared_ptr<NavMeshEdge>> lastEdge;
	std::unordered_map<std::shared_ptr<NavMeshEdge>, std::shared_ptr<NavMeshNode>> enteringNode;
	std::unordered_set<std::shared_ptr<NavMeshEdge>> closedList;
	std::priority_queue<std::pair<float, std::shared_ptr<NavMeshEdge>>> openList;

	std::vector<glm::vec3> path = {};

	bool getEnd = (startNode == endNode);
	
	if (getEnd) {
		std::cout << "get end" << std::endl;
		path.push_back(endPos);
		return path;
	}

	for (const auto& edge : startNode->connectedEdges) {
		if (!edge->isInterior) continue;
		openList.push({getEuclidianDistance(edge, startPos) + getEuclidianDistance(edge, endPos), edge});
		lastEdge[edge] = edge;
		std::shared_ptr<NavMeshNode> nextNode;
		if (edge->connectedNodes[0] == startNode) nextNode = edge->connectedNodes[1];
		else nextNode = edge->connectedNodes[0];
		enteringNode[edge] = nextNode;
	}
	std::shared_ptr<NavMeshEdge> endEdge;
	while (!getEnd && !openList.empty()) {
		auto cost = openList.top().first;
		auto currentEdge = openList.top().second;
		openList.pop();
		if (enteringNode[currentEdge] == endNode) {
			getEnd = true;
			endEdge = currentEdge;
			break;
		}
		if (closedList.find(currentEdge) != closedList.end()) continue;  // In closed list
		closedList.insert(currentEdge);
		auto costH = getEuclidianDistance(currentEdge, endPos);
		for (const auto& connectedNode : enteringNode[currentEdge]->connectedNodes) {
			const auto& connectedEdge = commonEdges[connectedNode][enteringNode[currentEdge]];
			if (closedList.find(connectedEdge) != closedList.end()) continue;  // In closed list
			float newCost = cost - costH + getEuclidianDistance(connectedEdge, currentEdge) + getEuclidianDistance(connectedEdge, endPos);
			if (lowestCost.find(connectedEdge) == lowestCost.end() || lowestCost[connectedEdge] > newCost) {
				lowestCost[connectedEdge] = newCost;
				lastEdge[connectedEdge] = currentEdge;
				enteringNode[connectedEdge] = connectedNode;
			}
		}
	}

	if (!getEnd) {
		std::cerr << "A* failed: heap is empty before getting to the end!" << std::endl;
		return path;
	}

	// Get shortest path
	path.push_back(endPos);
	auto currentEdge = endEdge;
	while (lastEdge[currentEdge] != currentEdge) {
		path.push_back((vertexPositions[currentEdge->vertexIndex1] - vertexPositions[currentEdge->vertexIndex2]) / 2.0f);
		currentEdge = lastEdge[currentEdge];
	}
	path.push_back((vertexPositions[currentEdge->vertexIndex1] - vertexPositions[currentEdge->vertexIndex2]) / 2.0f);
	std::reverse(path.begin(), path.end());

	return path;
}

float NavMesh::rayCast(glm::vec3& origin, glm::vec3 direction, std::shared_ptr<NavMeshNode>& castedNode)
{
	float res = -1;

	for (const auto& node : nodes) {
		if (glm::dot(direction, node->normal) == 0) continue;
		float t = glm::dot(vertexPositions[node->positionIndex[0]] - origin, node->normal) / glm::dot(direction, node->normal);
		if (t >= 0 && (res < 0 || res > t)) {
			res = t;
			castedNode = node;
		}
	}

	return res;
}