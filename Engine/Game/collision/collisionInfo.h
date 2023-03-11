#pragma once

#include <glm/glm.hpp>

struct CollisionInfo
{
	CollisionInfo() {}

	CollisionInfo(float t, glm::vec3 normal, glm::vec3 contact)
	{
		this->t = t;
		this->normal = normal;
		this->contact = contact;
	}

	float t = -1.0f;  // If < 1, there is no collision
	glm::vec3 normal = glm::vec3(1, 0, 0);
	glm::vec3 contact = glm::vec3(0, 0, 0);
};
