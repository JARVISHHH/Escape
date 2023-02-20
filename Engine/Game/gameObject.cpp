#include "gameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

bool GameObject::addComponent(std::string tag, std::shared_ptr<GameComponent> component)
{
	components[tag] = component;
	return true;
}

bool GameObject::removeComponent(std::string tag)
{
	components[tag] = nullptr;
	return true;
}