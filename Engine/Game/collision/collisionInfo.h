#pragma once

#include <glm/glm.hpp>

#include <memory>

class EnvironmentComponent;

struct CollisionInfo
{
	float t = -1.0f;  // If < 0, there is no collision
	glm::vec4 normal = glm::vec4(1, 0, 0, 0);
	glm::vec4 contact = glm::vec4(0, 0, 0, 1);
	glm::vec4 center = glm::vec4(1, 0, 0, 0);

	std::weak_ptr<EnvironmentComponent> environmentComponent;
};
