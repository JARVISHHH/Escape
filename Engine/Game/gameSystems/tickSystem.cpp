#include "tickSystem.h"
#include <Engine/Game/gameComponent.h>

TickSystem::TickSystem()
	: GameSystem("tick")
{
}

TickSystem::TickSystem(std::shared_ptr<GameWorld> gameWorld)
	: GameSystem(gameWorld, "tick")
{
}

void TickSystem::update(double seconds)
{
	for (const auto& component : components) {
		component->update(seconds);
	}
}

void TickSystem::addComponent(std::shared_ptr<GameComponent> component)
{
	components.push_back(component);
}

void TickSystem::updateComponents()
{
	for (int i = components.size() - 1; i >= 0; i--)
		if (!components[i]->getGameObject()->getActiveStatus())
			components.erase(components.begin() + i);
}