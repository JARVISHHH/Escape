#pragma once

#include "Engine/Game/gameSystem.h"

class DrawSystem : public GameSystem {
public:
	DrawSystem();
	DrawSystem(std::shared_ptr<GameWorld> gameWorld);

	void draw();
	void drawPhong();
	void drawText();

	bool addComponent(std::shared_ptr<GameComponent> gameComponent);
	void updateComponents();

protected:
	std::vector<std::shared_ptr<GameComponent>> components;
	std::vector<std::shared_ptr<GameComponent>> waitingList;
};
