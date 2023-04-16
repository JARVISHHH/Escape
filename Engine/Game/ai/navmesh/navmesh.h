#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <queue>
#include <unordered_set>

class NavMeshEdge;
class NavMeshNode;

class NavMesh : public std::enable_shared_from_this<NavMesh> {
public:
	NavMesh(std::string filePath);
	void bake();
	std::vector<glm::vec3> pathFinding(glm::vec3 start, glm::vec3 end);

	std::vector<glm::vec3> aStar(glm::vec3 startPos, glm::vec3 endPos, std::shared_ptr<NavMeshNode> startNode, std::shared_ptr<NavMeshNode> endNode);
	bool insideTriangle(glm::vec3 pos, std::shared_ptr<NavMeshNode> node);
	float rayCast(glm::vec3& start, glm::vec3 direction, std::shared_ptr<NavMeshNode>& castedNode);

	float getEuclidianDistance(std::shared_ptr<NavMeshNode> node, glm::vec3 target);
	float getEuclidianDistance(std::shared_ptr<NavMeshEdge> edge, glm::vec3 target);
	float getEuclidianDistance(std::shared_ptr<NavMeshNode> node1, std::shared_ptr<NavMeshNode> node2);
	float getEuclidianDistance(std::shared_ptr<NavMeshEdge> edge1, std::shared_ptr<NavMeshEdge> edge2);

	std::string filePath;

	std::vector<glm::vec3> vertexPositions;
	std::vector<std::shared_ptr<NavMeshNode>> nodes;
	std::vector<std::shared_ptr<NavMeshEdge>> edges;
	//float commonEdges;
	std::unordered_map<std::shared_ptr<NavMeshNode>, std::unordered_map<std::shared_ptr<NavMeshNode>, std::shared_ptr<NavMeshEdge>>> commonEdges;

private:


};