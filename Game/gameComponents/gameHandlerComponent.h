#pragma once

#include <Engine/Game/gameComponent.h>
#include <Game/components.h>
#include <Engine/UIKit/uiElement.h>

enum GameState {
	OnHold,
	Running,
	Win,
	Lose
};

class GameHandlerComponent : public GameComponent {
public:
	GameHandlerComponent(float maxTime, std::shared_ptr<UIElement> time, std::shared_ptr<UIElement> resultImage, std::shared_ptr<UIElement> scoreImage);

	void start() override;
	void update(double seconds) override;
	void drawText() override;

	void killEnemy();
	void addScore(int score);
	void endGame(bool win);

private:

	GameState gameState;
	float maxTime = 60;
	float leftTime;
	std::shared_ptr<UIElement> time;

	std::string result = "";
	std::shared_ptr<UIElement> resultImage;

	int score = 0;
	int killedEnemies = 0;
	std::shared_ptr<UIElement> scoreImage;

	void calculateScore();
};