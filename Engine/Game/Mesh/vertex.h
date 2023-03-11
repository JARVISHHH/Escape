#pragma once

#include <glm/glm.hpp>

class Vertex {
public:
	Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);

	glm::vec4 setPosition(glm::vec4 position) { this->position = position; }
	glm::vec4 getPosition() { return position; }

private:
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
};