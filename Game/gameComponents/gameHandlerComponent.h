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
	GameHandlerComponent(float maxTime = 60);

	void start() override;
	void update(double seconds) override;
	void drawText() override;

	void endGame(bool win);

private:

	GameState gameState;
	float maxTime = 60;
	float leftTime;

	std::string result = "";

	int score = 0;
};