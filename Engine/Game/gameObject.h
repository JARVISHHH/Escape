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

	template <class T> bool addComponent(std::string tag);
	bool addComponent(std::string tag, std::shared_ptr<GameComponent> component);
	bool removeComponent(std::string tag);
	template <class T> std::shared_ptr<T> getComponent(std::string tag);

protected:
	std::unordered_map<std::string, std::shared_ptr<GameComponent>> components;
};

template<class T>
inline bool GameObject::addComponent(std::string tag)
{
	components[tag] = std::make_shared<T>();
	return false;
}

template<class T>
std::shared_ptr<T> GameObject::getComponent(std::string tag)
{
	if (components.find(tag) == components.end()) return nullptr;
	else return static_pointer_cast<T>(components[tag]);
}