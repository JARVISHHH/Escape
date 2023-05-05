#include "characterCollisionResponse.h"

#include "Engine/Game/components/physicsComponent.h"
#include <Game/gameComponents/characterShoot.h>

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
	}
	else if (otherComponent->getGameObject()->getGameObject() != nullptr && otherComponent->getGameObject()->getGameObject()->getTag().compare("treasure") == 0) {
		otherComponent->getGameObject()->getGameObject()->setActiveStatus(false);
		if (otherComponent->getGameObject()->getTag().compare("coin") == 0) {
			std::cout << "coin" << std::endl;
			gameHandler->addScore(50);
		}
		else if (otherComponent->getGameObject()->getTag().compare("bullet") == 0) {
			std::cout << "bullet" << std::endl;
			getGameObject()->getComponent<CharacterShoot>("characterShoot")->bulletTag = "superBullet";
		}
		else if (otherComponent->getGameObject()->getTag().compare("health") == 0) {
			std::cout << "health" << std::endl;
			healthComponent->heal(2);
		}
	}
	//else if (otherComponent->getGameObject()->getTag().compare("spike") == 0) {
	//	gameHandler->endGame(false);
	//}
}

void CharacterCollisionResponse::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	auto physicsComponent = getGameObject()->getComponent<PhysicsComponent>("physics");
	for (auto collision : collisions) {
		if (collision->normal.y > 0)
		{
			if (physicsComponent != nullptr) physicsComponent->setVelocity(glm::vec3(physicsComponent->getVelocity().x, 0.0f, physicsComponent->getVelocity().z));
		}
		if (collision->environmentComponent.lock()->getGameObject()->getTag().compare("spike") == 0) {
			gameHandler->endGame(false);
		}
	}
}