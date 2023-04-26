#include "characterCollisionResponse.h"

#include "Engine/Game/components/physicsComponent.h"

void CharacterCollisionResponse::start()
{
	healthComponent = gameObject->getComponent<HealthComponent>("health");
}

void CharacterCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (mtv.y > 0) {
		auto physicsComponent = gameObject->getComponent<PhysicsComponent>("physics");
		if (physicsComponent != nullptr && physicsComponent->getVelocity().y <= 0)
			physicsComponent->applyVelocity(glm::vec3(0, -physicsComponent->getVelocity().y, 0));
	}

	if (otherComponent->getGameObject()->getTag().compare("goal") == 0) {
		gameObject->getGameWorld()->setWin(true);
		gameObject->getGameWorld()->setFinish(true);
	}
	else if (otherComponent->getGameObject()->getTag().compare("enemy") == 0) {
		healthComponent->damage(2);
		//if (healthComponent->getCurrentHealth() <= 0) {
		//	gameObject->getGameWorld()->setWin(false);
		//	gameObject->getGameWorld()->setFinish(true);
		//}
		//gameObject->getGameWorld()->setWin(false);
		//gameObject->getGameWorld()->setFinish(true);
		//std::cout << "collide with enemy" << std::endl;
	}
	//else if (otherComponent->getGameObject()->getTag().compare("enemy") == 0) {
	//	gameObject->getGameWorld()->setWin(false);
	//	gameObject->getGameWorld()->setFinish(true);
	//}
}