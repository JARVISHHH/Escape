#pragma once

#include <unordered_map>
#include <memory>

#include "gameWorld.h"

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	template <class T> bool addComponent(std::string tag);
	bool addComponent(std::string tag, std::shared_ptr<Component> component);
	bool removeComponent(std::string tag);
	template <class T> std::shared_ptr<T> getComponent(std::string tag);

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components;
};