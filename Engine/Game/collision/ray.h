#pragma once

#include <glm/glm.hpp>

struct Ray
{
	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		this->origin = glm::vec4(origin[0], origin[1], origin[2], 1);
		this->direction = glm::vec4(direction[0], direction[1], direction[2], 0);
	}

	glm::vec4 origin = glm::vec4(0, 0, 0, 1);
	glm::vec4 direction = glm::vec4(1, 0, 0, 0);
};