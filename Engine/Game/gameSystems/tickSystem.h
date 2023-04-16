#pragma once

#include "Engine/Game/gameSystem.h"

class TickSystem : public GameSystem {
public:
	TickSystem();

	void update(double seconds);
	void addComponent(std::shared_ptr<GameComponent> component);
	void updateComponents();

private:
	std::vector<std::shared_ptr<GameComponent>> components;

};