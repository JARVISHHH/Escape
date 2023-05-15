#include "uiElement.h"

void UIElement::draw()
{
	for (auto& child : children)
		child->draw();
}

void UIElement::click()
{
	for (auto& child : children)
		child->draw();
}

void UIElement::add(std::shared_ptr<UIElement> element)
{
	children.push_back(element);
}