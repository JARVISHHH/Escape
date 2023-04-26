#include "gameHandlerComponent.h"

GameHandlerComponent::GameHandlerComponent(std::string targetTag, float maxTime)
	: GameComponent("gameHandler"), targetTag(targetTag), maxTime(maxTime), leftTime(maxTime)
{
	gameState = GameState::OnHold;
}

void GameHandlerComponent::start() {
	target = gameObject->getGameWorld()->getGameObject(targetTag)->getComponent<HealthComponent>("health");
	gameState = GameState::Running;
	gameObject->getGameWorld()->setIsStop(false);
}

void GameHandlerComponent::update(double seconds)
{
	if (gameState != GameState::Running) return;
	if (leftTime <= 0) {
		leftTime = maxTime;
		endGame(true);
	}
	else if (target->getCurrentHealth() <= 0) {
		endGame(false);
	}
	else {
		leftTime -= seconds;
	}
}

void GameHandlerComponent::endGame(bool win)
{
	if (win) gameState = GameState::Win;
	else gameState = GameState::Lose;
	gameObject->getGameWorld()->setWin(win);
	gameObject->getGameWorld()->setFinish(true);
}