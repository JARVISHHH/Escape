#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include <GL/glew.h>

#include <Engine/screen.h>

class UIElement {
public:
	virtual void draw();
	virtual void click();

	void add(std::shared_ptr<UIElement> element);

protected:
	glm::vec3 position;  // Absolute top-left position
	glm::vec3 localPosition;  // Relative top-left position
	std::vector<std::shared_ptr<UIElement>> children;  // Children elements
};