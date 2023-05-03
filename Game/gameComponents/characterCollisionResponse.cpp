#include "characterCollisionResponse.h"

#include "Engine/Game/components/physicsComponent.h"

CharacterCollisionResponse::CharacterCollisionResponse(std::shared_ptr<GameObject> gameHandlerObject)
	:gameHandlerObject(gameHandlerObject)
{
}

void CharacterCollisionResponse::start()
{
	healthComponent = getGameObject()->getComponent<HealthComponent>("health");
	gameHandler = gameHandlerObject->getComponent<GameHandlerComponent>("gameHandler");
}

void CharacterCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (mtv.y > 0) {
		auto physicsComponent = getGameObject()->getComponent<PhysicsComponent>("physics");
		if (physicsComponent != nullptr && physicsComponent->getVelocity().y <= 0)
			physicsComponent->applyVelocity(glm::vec3(0, -physicsComponent->getVelocity().y, 0));
	}

	if (otherComponent->getGameObject()->getTag().compare("goal") == 0) {
		gameHandler->endGame(true);
	}
	else if (otherComponent->getGameObject()->getTag().compare("enemy") == 0) {
		healthComponent->damage(2);
		//if (healthComponent->getCurrentHealth() <= 0) {
		//	gameObject->getGameWorld()->setWin(false);
		//	gameObject->getGameWorld()->setFinish(true);
		//}
	}
}