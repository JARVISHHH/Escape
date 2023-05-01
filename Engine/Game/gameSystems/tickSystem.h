#pragma once

#include "Engine/Game/gameSystem.h"

class TickSystem : public GameSystem {
public:
	TickSystem();
	TickSystem(std::shared_ptr<GameWorld> gameWorld);
	~TickSystem();

	void update(double seconds);
	void addComponent(std::shared_ptr<GameComponent> component);
	void updateComponents();

private:
	std::vector<std::shared_ptr<GameComponent>> components;
	std::vector<std::shared_ptr< GameComponent>> waitingList;
};