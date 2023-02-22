#include "physicsComponent.h"
#include <Engine/Game/components/transformComponent.h>

PhysicsComponent::PhysicsComponent()
	: GameComponent("physics")
{

}

void PhysicsComponent::update(double seconds)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	auto modelTransform = transformComponent->getModelTransform();
	auto bottomPosition = modelTransform->getPos().y - 0.5 * modelTransform->getScale().y;

	if (bottomPosition > 0) {
		velocity -= glm::vec3(0, gravity * seconds, 0);
	}

	auto moveDistance = velocity * (float)seconds;
	if (moveDistance.y < 0 && bottomPosition < std::abs(moveDistance.y)) {
		velocity.y = 0;
		moveDistance.y = -bottomPosition;
	}
	modelTransform->translate(glm::vec3(0, moveDistance.y, 0));
}

float PhysicsComponent::getGravity()
{
	return gravity;
}

glm::vec3 PhysicsComponent::getVelocity()
{
	return velocity;
}

void PhysicsComponent::applyVelocity(glm::vec3 plusVelocity)
{
	this->velocity += plusVelocity;
}