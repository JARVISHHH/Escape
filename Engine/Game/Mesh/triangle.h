#pragma once

#include <memory>
#include <vector>

#include "vertex.h"
#include "Engine/Game/collision/collisionInfo.h"
#include <Engine/Game/collision/ray.h>


class Triangle {
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	Triangle(std::shared_ptr<Vertex>* v);

	glm::vec4 calculateFaceNormal();
	CollisionInfo intersect(glm::mat4x4 transformMatrix, const Ray& ray);

private:
	std::vector<std::shared_ptr<Vertex>> v;
	glm::vec4 faceNormal;
};