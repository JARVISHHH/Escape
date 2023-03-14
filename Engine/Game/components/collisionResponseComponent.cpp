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

void CollisionResponseComponent::doCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	// resolve
	this->resolveCollision(collisions, curPos);

	// response
	this->responseCollision(collisions, curPos);
}

void CollisionResponseComponent::resolveCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	if (isStatic) return;
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (otherComponent == nullptr || otherComponent->isStatic) transformComponent->getModelTransform()->translate(mtv);
	else transformComponent->getModelTransform()->translate(mtv / 2.0f);
}

void CollisionResponseComponent::resolveCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (isStatic) return;
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	transformComponent->getModelTransform()->setPos(curPos);
}

void CollisionResponseComponent::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{

}

void CollisionResponseComponent::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	transformComponent->setOnGround(false);
	for (auto collision : collisions)
		if (collision->normal.y > 0)
			transformComponent->setOnGround(true);
}