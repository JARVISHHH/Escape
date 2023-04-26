#pragma once

#include <Engine/Game/gameComponent.h>
#include <Game/components.h>

enum GameState {
	OnHold,
	Running,
	Win,
	Lose
};

class GameHandlerComponent : public GameComponent {
public:
	GameHandlerComponent(std::string targetTag, float maxTime = 60);

	void start() override;
	void update(double seconds) override;

	void endGame(bool win);

private:

	GameState gameState;
	float maxTime = 60;
	float leftTime;

	std::string targetTag;
	std::shared_ptr<HealthComponent> target;
};