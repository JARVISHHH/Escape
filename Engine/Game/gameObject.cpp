#include "gameObject.h"

#include "gameComponent.h"

GameObject::GameObject(std::string tag)
{
	this->tag = tag;
}

GameObject::~GameObject()
{
	//std::cout << "game object delete" << std::endl;
}

void GameObject::start()
{
	for (auto it = components.begin(); it != components.end(); it++) {
		for (const auto& component : it->second) {
			component->start();
		}
	}
}

std::string GameObject::getTag()
{
	return tag;
}

std::shared_ptr<GameWorld> GameObject::getGameWorld()
{
	return gameWorld.lock();
}

void GameObject::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
}

std::shared_ptr<GameObject> GameObject::getGameObject()
{
	return gameObject.lock();
}

void GameObject::setGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObject = gameObject;
}

bool GameObject::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
	if (gameObject->getGameObject() != nullptr)
		gameObject->getGameObject()->removeGameObject(gameObject);
	gameObject->setGameObject(shared_from_this());
	return true;
}

bool GameObject::removeGameObject(std::shared_ptr<GameObject> gameObject)
{
	int i = 0;
	while (i < gameObjects.size()) {
		if (gameObjects[i] == gameObject) gameObjects.erase(gameObjects.begin() + i);
		else i++;
	}
	return true;
}

bool GameObject::addComponent(std::shared_ptr<GameComponent> component)
{
	component->setGameObject(shared_from_this());
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
	return gameWorld.lock()->getCamera();
}

void GameObject::setActiveStatus(bool activeStatus)
{
	this->activeStatus = activeStatus;
	for (auto gameObject : gameObjects)
		gameObject->setActiveStatus(activeStatus);
}

bool GameObject::getActiveStatus()
{
	return activeStatus;
}