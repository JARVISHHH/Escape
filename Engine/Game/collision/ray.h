#pragma once

#include <glm/glm.hpp>

struct Ray
{
	Ray(glm::vec3 origin, glm::vec3 direction)
		:origin(origin), direction(direction) {}

	glm::vec3 origin = glm::vec3(0, 0, 0);
	glm::vec3 direction = glm::vec3(1, 0, 0);
};