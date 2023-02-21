#include "collisionResponseComponent.h"
#include <Engine/Game/components/transformComponent.h>

CollisionResponseComponent::CollisionResponseComponent()
	: GameComponent("collisionResponse")
{
}

CollisionResponseComponent::CollisionResponseComponent(bool isStatic)
	: GameComponent("collisionResponse"), isStatic(isStatic)
{
}

void CollisionResponseComponent::doCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	// resolve
	this->resolveCollision(otherComponent, mtv);

	// response
	this->responseCollision(otherComponent, mtv);
}

void CollisionResponseComponent::resolveCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (isStatic) return;
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (otherComponent->isStatic) transformComponent->getModelTransform()->translate(mtv);
	else transformComponent->getModelTransform()->translate(mtv / 2.0f);
}

void CollisionResponseComponent::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{

}