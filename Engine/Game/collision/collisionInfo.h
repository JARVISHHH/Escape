#pragma once

#include <glm/glm.hpp>

#include <memory>

class EnvironmentComponent;

struct CollisionInfo
{
	float t = -1.0f;  // If < 0, there is no collision
	glm::vec4 normal = glm::vec4(1, 0, 0, 0);
	glm::vec4 contact = glm::vec4(0, 0, 0, 1);  // Contact of the collision
	glm::vec4 center = glm::vec4(1, 0, 0, 0);  // Center of the object at collision position

	std::weak_ptr<EnvironmentComponent> environmentComponent;
};
