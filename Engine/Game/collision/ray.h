#pragma once

#include <glm/glm.hpp>

struct Ray
{
	Ray(glm::vec3 origin, glm::vec3 endPoint)
	{
		this->origin = glm::vec4(origin[0], origin[1], origin[2], 1);
		this->endPoint = glm::vec4(endPoint[0], endPoint[1], endPoint[2], 0);
		this->direction = this->endPoint - this->origin;
	}

	glm::vec4 origin = glm::vec4(0, 0, 0, 1);
	glm::vec4 direction = glm::vec4(1, 0, 0, 0);
	glm::vec4 endPoint = glm::vec4(0, 0, 0, 1);
};