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

TickSystem::~TickSystem()
{
	//std::cout << "tick system delete" << std::endl;
}

void TickSystem::update(double seconds)
{
	updateComponents();
	for (const auto& component : components) {
		component->update(seconds);
	}
}

void TickSystem::addComponent(std::shared_ptr<GameComponent> component)
{
	waitingList.push_back(component);
}

void TickSystem::updateComponents()
{
	components.insert(components.end(), waitingList.begin(), waitingList.end());
	waitingList.clear();
	for (int i = components.size() - 1; i >= 0; i--)
		if (!components[i]->isActive())
			components.erase(components.begin() + i);
}