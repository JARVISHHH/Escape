#include "characterHealth.h"

CharacterHealth::CharacterHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject)
	:HealthComponent(maxHealth), gameHandlerObject(gameHandlerObject)
{
}

void CharacterHealth::start()
{
	gameHandler = gameHandlerObject->getComponent<GameHandlerComponent>("gameHandler");
	drawComponent = getGameObject()->getComponent<DrawComponent>("draw");
}

void CharacterHealth::update(double seconds)
{
	if (leftImmuneTime >= 0) {
		leftImmuneTime -= seconds;
		if (lastImmune - leftImmuneTime >= 0.1) {
			drawComponent->setAlpha(0.75 - (drawComponent->getAlpha() - 0.75));
			lastImmune = lastImmune - 0.1;
		}
	}
	else {
		drawComponent->setAlpha(1.0f);
	}
}

bool CharacterHealth::damage(int damageNumber)
{
	if (leftImmuneTime <= 0) {
		currentHealth = std::max(currentHealth - damageNumber, 0.0f);
		std::cout << "get damage, left health: " << currentHealth << std::endl;
		leftImmuneTime = immuneTime;
		lastImmune = immuneTime;
	}
	if (currentHealth <= 0) gameHandler->endGame(false);
	return true;
}