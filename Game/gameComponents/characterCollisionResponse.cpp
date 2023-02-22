#include "characterCollisionResponse.h"

#include "Engine/Game/components/physicsComponent.h"

void CharacterCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (mtv.y == 0) return;
	auto physicsComponent = gameObject->getComponent<PhysicsComponent>("physics");
	if (physicsComponent == nullptr) return;
	if (physicsComponent->getVelocity().y > 0) return;
	physicsComponent->applyVelocity(glm::vec3(0, -physicsComponent->getVelocity().y, 0));
}