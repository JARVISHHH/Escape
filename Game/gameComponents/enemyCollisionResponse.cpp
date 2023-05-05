#include "enemyCollisionResponse.h"

EnemyCollisionResponse::EnemyCollisionResponse()
	: CollisionResponseComponent()
{
}

EnemyCollisionResponse::EnemyCollisionResponse(bool isStatic)
	: CollisionResponseComponent(isStatic)
{
}

void EnemyCollisionResponse::start()
{
	healthComponent = getGameObject()->getComponent<HealthComponent>("health");
}

void EnemyCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (otherComponent->getGameObject()->getTag().compare("bullet") == 0) {
		healthComponent->damage(5);
	}
	else if (otherComponent->getGameObject()->getTag().compare("superBullet") == 0) {
		healthComponent->damage(10);
	}
}