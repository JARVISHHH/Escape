#pragma once

#include <glm/glm.hpp>

struct CollisionInfo
{
	CollisionInfo() {}

	CollisionInfo(float t, glm::vec4 normal, glm::vec4 contact)
	{
		this->t = t;
		this->normal = normal;
		this->contact = contact;
	}

	float t = -1.0f;  // If < 1, there is no collision
	glm::vec4 normal = glm::vec4(1, 0, 0, 0);
	glm::vec4 contact = glm::vec4(0, 0, 0, 1);
};
