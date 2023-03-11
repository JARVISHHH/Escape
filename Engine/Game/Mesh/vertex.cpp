#include "vertex.h"

Vertex::Vertex(glm::vec3 position)
{
	this->position = glm::vec4(position[0], position[1], position[2], 1);
	normal = glm::vec3(1, 0, 0);
	uv = glm::vec2(1, 0);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) 
	: normal(normal), uv(uv)
{
	this->position = glm::vec4(position[0], position[1], position[2], 1);
}