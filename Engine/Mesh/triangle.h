#pragma once

#include <memory>
#include <vector>

#include "vertex.h"
#include "Engine/Game/collision/collisionInfo.h"
#include <Engine/Game/collision/ray.h>
#include <Graphics/modeltransform.h>


class Triangle {
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	Triangle(std::shared_ptr<Vertex>* v);

	const std::vector<std::shared_ptr<Vertex>> getV() { return v; }

	glm::vec4 calculateFaceNormal();
	// All these positions and rays should be in sphere space
	bool isInside(glm::vec4 pos);
	bool isInside(std::vector<glm::vec4>& sphereSpacePos, glm::vec4 pos);
	bool intersectInterior(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo);
	bool intersectEdges(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo);
	bool intersectVertices(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo);
	std::shared_ptr<CollisionInfo> intersect(glm::mat4x4 transformMatrix, glm::mat4x4 triangleTransformMatrix, std::shared_ptr<Ray> sphereSpaceRay);

private:
	std::vector<std::shared_ptr<Vertex>> v;
	glm::vec4 faceNormal;
};