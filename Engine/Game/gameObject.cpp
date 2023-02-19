#include "gameObject.h"

#include "component.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

template<class T>
bool GameObject::addComponent(std::string tag)
{
	components[tag] = std::make_shared<T>();
	return true;
}

bool GameObject::addComponent(std::string tag, std::shared_ptr<Component> component)
{
	components[tag] = component;
	return true;
}

bool GameObject::removeComponent(std::string tag)
{
	components[tag] = nullptr;
	return true;
}

template<class T>
std::shared_ptr<T> GameObject::getComponent(std::string tag)
{
	if (components.find(tag) == components.end()) return nullptr;
	else return components[tag];
}