#include "vertex.h"

Vertex::Vertex(glm::vec3 position)
{
	this->position = glm::vec4(position[0], position[1], position[2], 1);
}