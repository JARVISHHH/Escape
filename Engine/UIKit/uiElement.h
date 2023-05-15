#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include <GL/glew.h>

#include <Engine/screen.h>

class UIElemnet {
public:
	virtual void draw();
	virtual void click();

	void add(std::shared_ptr<UIElemnet> element);

protected:
	glm::vec3 position;  // Absolute bottom-left position
	glm::vec3 localPosition;  // Relative bottom-left position
	std::vector<std::shared_ptr<UIElemnet>> children;  // Children elements
};