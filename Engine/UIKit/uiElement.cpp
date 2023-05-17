#include "uiElement.h"

void UIElement::draw()
{
	if (!show) return;
	for (auto& child : children)
		child->draw();
}

void UIElement::mousePosEvent(double xpos, double ypos)
{
	for (auto& child : children)
		child->mousePosEvent(xpos, ypos);
}

void UIElement::mouseButtonEvent(int button, int action)
{
	for (auto& child : children)
		child->mouseButtonEvent(button, action);
}

void UIElement::resize(glm::vec2 scale)
{
	auto newLocalPosition = glm::vec3(localPosition.x * scale.x, localPosition.y * scale.y, localPosition.z);
	setLocalPosition(newLocalPosition);
	size = glm::vec3(size[0] * scale[0], size[1] * scale[1], size[2]);
	for (auto element : children)
		element->resize(scale);
}

void UIElement::setPosition(glm::vec3 position)
{
	this->localPosition += position - this->position;
	this->position = position;
	for (auto element : children)
		element->updatePosition();
}

glm::vec3 UIElement::getPosition()
{
	return position;
}

void UIElement::setLocalPosition(glm::vec3 localPosition)
{
	this->position += localPosition - this->localPosition;
	this->localPosition = localPosition;
	for (auto element : children)
		element->updatePosition();
}

glm::vec3 UIElement::getLocalPosition()
{
	return localPosition;
}

void UIElement::setSize(glm::vec3 size)
{
	this->size = size;
}

glm::vec3 UIElement::getSize()
{
	return size;
}

RenderType UIElement::getRenderType()
{
	return renderType;
}

glm::vec3 UIElement::getColor()
{
	return color;
}

void UIElement::setMaterial(std::string materialName)
{
	this->materialName = materialName;
}

std::string UIElement::getMaterialName()
{
	return materialName;
}

void UIElement::setShow(bool show)
{
	this->show = show;
}

void UIElement::add(std::shared_ptr<UIElement> element)
{
	children.push_back(element);
	element->setParent(shared_from_this());
}

void UIElement::setParent(std::shared_ptr<UIElement> parent)
{
	this->parent = parent;
	updatePosition();
}

std::shared_ptr<UIElement> UIElement::getParent()
{
	return parent.lock();
}

void UIElement::updatePosition()
{
	position = getParent()->position + localPosition;
	for (auto element : children)
		element->updatePosition();
}