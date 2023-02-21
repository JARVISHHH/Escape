#include "gameObject.h"

#include "gameComponent.h"

void GameObject::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
}

bool GameObject::addComponent(std::shared_ptr<GameComponent> component)
{
	component->setGameObject(std::shared_ptr<GameObject>(this));
	auto componentTag = component->getTag();
	if (components.find(componentTag) == components.end()) components[componentTag] = std::vector<std::shared_ptr<GameComponent>>();
	components[componentTag].push_back(component);
	return true;
}

bool GameObject::removeComponent(std::string tag, std::shared_ptr<GameComponent> component)
{
	if (components.find(tag) == components.end()) return true;
	int i = 0;
	while (i < components.size()) {
		if (components[tag][i] == component) components[tag].erase(components[tag].begin() + i);
		else i++;
	}
	return true;
}

std::shared_ptr<Camera> GameObject::getCamera()
{
	return gameWorld->getCamera();
}