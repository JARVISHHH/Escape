#include "collisionResponseComponent.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/components/physicsComponent.h>

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
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	if (otherComponent == nullptr || otherComponent->isStatic) transformComponent->getModelTransform()->translate(mtv);
	else transformComponent->getModelTransform()->translate(mtv / 2.0f);
}

void CollisionResponseComponent::resolveCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (isStatic || collisions.size() <= 0) return;
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	//auto objectTransformMatrix = transformComponent->getModelTransform()->getModelMatrix();
	//auto inverseObject = glm::inverse(objectTransformMatrix);
	//auto collisionObjectSpacePos = inverseObject * glm::vec4(curPos[0], curPos[1], curPos[2], 1);
	//curpos = collisionObjectSpacePos + (glm::vec4(0, 0, 0, 1) -)
	transformComponent->getModelTransform()->setPos(curPos);
}

void CollisionResponseComponent::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{

}

void CollisionResponseComponent::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	auto physicsComponent = getGameObject()->getComponent<PhysicsComponent>("physics");
	//transformComponent->setOnGround(false);
	for (auto collision : collisions)
		if (collision->normal.y > 0) 
		{
			//transformComponent->setOnGround(true);
			if (physicsComponent != nullptr) physicsComponent->setVelocity(glm::vec3(physicsComponent->getVelocity().x, 0.0f, physicsComponent->getVelocity().z));
		}
}