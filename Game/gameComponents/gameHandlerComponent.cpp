#include "gameHandlerComponent.h"

GameHandlerComponent::GameHandlerComponent(float maxTime, std::shared_ptr<UIElement> time, std::shared_ptr<UIElement> resultImage)
	: GameComponent("gameHandler"), maxTime(maxTime), leftTime(maxTime), time(time), resultImage(resultImage)
{
	gameState = GameState::OnHold;
}

void GameHandlerComponent::start() {
	gameState = GameState::Running;
	getGameObject()->getGameWorld()->setIsStop(false);
}

void GameHandlerComponent::update(double seconds)
{
	if (gameState != GameState::Running) return;
	if (leftTime <= 0) {
		auto size = time->getSize();
		time->setSize(glm::vec3(0, size[1], size[2]));
		endGame(false);
	}
	else {
		auto size = time->getSize();
		time->setSize(glm::vec3(size[0] / (leftTime) * (leftTime - seconds), size[1], size[2]));
		leftTime -= seconds;
	}
}

void GameHandlerComponent::drawText()
{
	//Global::graphics.drawUIText(Global::graphics.getFont("opensans"), result, glm::ivec2(50, 300), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 2.5f, 0.1f, glm::vec3(1, 0, 1));
	// Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Remaining time: " + std::to_string((int)(leftTime + 1)) + "s", glm::ivec2(0, 470), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 0, 1));
}

void GameHandlerComponent::killEnemy()
{
	killedEnemies++;
	std::cout << "kill enemy ++" << std::endl;
}

void GameHandlerComponent::addScore(int score)
{
	this->score += score;
}

void GameHandlerComponent::endGame(bool win)
{
	if (win) {
		gameState = GameState::Win;
		result = "You win!";
		resultImage->setMaterial("win");
		resultImage->setShow(true);
	}
	else {
		gameState = GameState::Lose;
		result = "You lose!";
		resultImage->setMaterial("die");
		resultImage->setShow(true);
	}
	getGameObject()->getGameWorld()->setWin(win);
	getGameObject()->getGameWorld()->setFinish(true);
}