#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include <GL/glew.h>

#include <Engine/screen.h>

class UIElement: public std::enable_shared_from_this<UIElement> {
public:

	virtual void draw();
	virtual void mouseButtonEvent(int button, int action);

	void setPosition(glm::vec3 position);
	void setLocalPosition(glm::vec3 localPosition);

	void add(std::shared_ptr<UIElement> element);
	void setParent(std::shared_ptr<UIElement> parent);

protected:
	glm::vec3 position = glm::vec3(0, 0, 0);  // Absolute top-left position
	glm::vec3 localPosition = glm::vec3(0, 0, 0);  // Relative top-left position

	std::weak_ptr<UIElement> parent;
	std::vector<std::shared_ptr<UIElement>> children;  // Children elements

	std::shared_ptr<UIElement> getParent();
	void updatePosition();
};