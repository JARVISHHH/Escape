#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include <GL/glew.h>

#include <Engine/screen.h>

enum RenderType {
	AlphaRender,
	ColorRender,
	TextureRender
};

class UIElement: public std::enable_shared_from_this<UIElement> {
public:

	virtual void draw();
	virtual void mousePosEvent(double xpos, double ypos);
	virtual void mouseButtonEvent(int button, int action);
	virtual void resize(glm::vec2 scale);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setLocalPosition(glm::vec3 localPosition);
	glm::vec3 getLocalPosition();
	void setSize(glm::vec3 size);
	glm::vec3 getSize();

	RenderType getRenderType();
	glm::vec3 getColor();
	void setMaterial(std::string materialName);
	std::string getMaterialName();
	void setShow(bool show);

	void add(std::shared_ptr<UIElement> element);
	void setParent(std::shared_ptr<UIElement> parent);

protected:
	glm::vec3 position = glm::vec3(0, 0, 0);  // Absolute top-left position
	glm::vec3 localPosition = glm::vec3(0, 0, 0);  // Relative top-left position
	glm::vec3 size = glm::vec3(0, 0, 0);

	RenderType renderType = TextureRender;
	glm::vec3 color = glm::vec3(0, 1, 0);
	std::string materialName = "button";

	std::weak_ptr<UIElement> parent;
	std::vector<std::shared_ptr<UIElement>> children;  // Children elements

	std::shared_ptr<UIElement> getParent();
	void updatePosition();

	bool show = true;
};