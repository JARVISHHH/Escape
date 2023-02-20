#include "gameObject.h"

#include "gameComponent.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

bool GameObject::addComponent(std::shared_ptr<GameComponent> component)
{
	component->setGameObject(std::shared_ptr<GameObject>(this));
	components[component->getTag()] = component;
	return true;
}

bool GameObject::removeComponent(std::string tag)
{
	components[tag] = nullptr;
	return true;
}