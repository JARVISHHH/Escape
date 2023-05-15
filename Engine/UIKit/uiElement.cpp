#include "uiElement.h"

void UIElemnet::draw()
{
	for (auto& child : children)
		child->draw();
}

void UIElemnet::click()
{
	for (auto& child : children)
		child->draw();
}

void UIElemnet::add(std::shared_ptr<UIElemnet> element)
{
	children.push_back(element);
}