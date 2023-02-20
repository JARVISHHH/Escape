#pragma once

#include <unordered_map>
#include <memory>

#include "gameWorld.h"

class GameComponent;

class GameObject
{
public:
	GameObject();
	~GameObject();

	template <class T> bool addComponent();
	bool addComponent(std::shared_ptr<GameComponent> component);
	bool removeComponent(std::string tag);
	template <class T> std::shared_ptr<T> getComponent(std::string tag);

protected:
	std::unordered_map<std::string, std::shared_ptr<GameComponent>> components;
};

template<class T>
inline bool GameObject::addComponent()
{
	std::shared_ptr<T> component = std::make_shared<T>();
	components[component->getTag()] = component;
	return false;
}

template<class T>
std::shared_ptr<T> GameObject::getComponent(std::string tag)
{
	if (components.find(tag) == components.end()) return nullptr;
	else return static_pointer_cast<T>(components[tag]);
}