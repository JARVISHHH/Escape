#include "uiElement.h"

void UIElement::draw()
{
	for (auto& child : children)
		child->draw();
}

void UIElement::mouseButtonEvent(int button, int action)
{
	for (auto& child : children)
		child->mouseButtonEvent(button, action);
}

void UIElement::setPosition(glm::vec3 position)
{
	this->localPosition += position - this->position;
	this->position = position;
	for (auto element : children)
		element->updatePosition();
}

void UIElement::setLocalPosition(glm::vec3 localPosition)
{
	this->position += localPosition - this->localPosition;
	this->localPosition = localPosition;
	for (auto element : children)
		element->updatePosition();
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