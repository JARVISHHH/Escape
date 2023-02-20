#include "gameObject.h"

#include "gameComponent.h"

void GameObject::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
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

std::shared_ptr<Camera> GameObject::getCamera()
{
	return gameWorld->getCamera();
}