#include "fallingCollisionResponse.h"
#include "Game/gameScreen.h"

void FallingCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	gameObject->setActiveStatus(false);
	gameObject->getGameWorld()->removeGameObject(gameObject);
	std::static_pointer_cast<GameScreen>(gameObject->getGameWorld()->getScreen())->addScore();
}