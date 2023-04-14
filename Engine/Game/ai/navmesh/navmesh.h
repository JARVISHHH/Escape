#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>

class NavMeshEdge;
class NavMeshNode;

class NavMesh : public std::enable_shared_from_this<NavMesh> {
public:
	NavMesh(std::string filePath);
	void bake();

	std::string filePath;

	std::vector<glm::vec3> vertexPositions;
	std::vector<std::shared_ptr<NavMeshNode>> nodes;
	std::vector<std::shared_ptr<NavMeshEdge>> edges;

private:


};