#pragma once

#include <unordered_map>
#include <memory>

#include "gameWorld.h"

class GameComponent;

class GameObject
{
public:
	void setGameWorld(std::shared_ptr<GameWorld> gameWorld);

	template <class T> bool addComponent();
	bool addComponent(std::shared_ptr<GameComponent> component);
	bool removeComponent(std::string tag, std::shared_ptr<GameComponent> component);
	template <class T> std::shared_ptr<T> getComponent(std::string tag);
	template <class T> std::vector<std::shared_ptr<T>> getComponents(std::string tag);

	std::shared_ptr<Camera> getCamera();

protected:
	std::shared_ptr<GameWorld> gameWorld;
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameComponent>>> components;
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
	else return static_pointer_cast<T>(components[tag][0]);
}

template<class T>
std::vector<std::shared_ptr<T>> GameObject::getComponents(std::string tag)
{
	if (components.find(tag) == components.end()) return std::vector<std::shared_ptr<T>>();
	std::vector<std::shared_ptr<T>> result(components[tag].size());
	for (int i = 0; i < components[tag].size(); i++)
		result[i] = static_pointer_cast<T>(components[tag][i]);
	return result;
}