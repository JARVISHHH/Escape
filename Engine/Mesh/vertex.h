#pragma once

#include <glm/glm.hpp>

class Vertex {
public:
	Vertex(glm::vec3 position);

	void setPosition(glm::vec4 position) { this->position = position; }
	glm::vec4 getPosition() { return position; }

private:
	glm::vec4 position;
};