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
		endGame(true);
	}
	else if (target->getCurrentHealth() <= 0) {
		endGame(false);
	}
	else {
		leftTime -= seconds;
	}
}

void GameHandlerComponent::drawText()
{
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), result, glm::ivec2(50, 300), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.5f, 0.1f, glm::vec3(1, 0, 1));
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Remaining time: " + std::to_string((int)(leftTime + 1)) + "s", glm::ivec2(0, 470), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}

void GameHandlerComponent::endGame(bool win)
{
	if (win) {
		gameState = GameState::Win;
		result = "You win!";
	}
	else {
		gameState = GameState::Lose;
		result = "You lose!";
	}
	gameObject->getGameWorld()->setWin(win);
	gameObject->getGameWorld()->setFinish(true);
}