#include "enemyHealth.h"

EnemyHealth::EnemyHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject)
	: HealthComponent(maxHealth), gameHandlerObject(gameHandlerObject)
{
}

void EnemyHealth::start()
{
	gameHandler = gameHandlerObject->getComponent<GameHandlerComponent>("gameHandler");
}

bool EnemyHealth::damage(int damageNumber)
{
	currentHealth = std::max(currentHealth - damageNumber, 0.0f);
	if (currentHealth <= 0) {
		gameHandler->killEnemy();
		getGameObject()->setActiveStatus(false);
	}
	return true;
}