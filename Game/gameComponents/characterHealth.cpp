#include "characterHealth.h"

CharacterHealth::CharacterHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject)
	:HealthComponent(maxHealth), gameHandlerObject(gameHandlerObject)
{
}

void CharacterHealth::start()
{
	gameHandler = gameHandlerObject->getComponent<GameHandlerComponent>("gameHandler");
}

bool CharacterHealth::damage(int damageNumber)
{
	if (leftImmuneTime <= 0) {
		currentHealth = std::max(currentHealth - damageNumber, 0.0f);
		std::cout << "get damage, left health: " << currentHealth << std::endl;
		leftImmuneTime = immuneTime;
	}
	if (currentHealth <= 0) gameHandler->endGame(false);
	return true;
}