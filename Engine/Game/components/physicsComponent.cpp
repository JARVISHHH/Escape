#include "physicsComponent.h"
#include <Engine/Game/components/transformComponent.h>

PhysicsComponent::PhysicsComponent()
	: GameComponent("physics")
{

}

void PhysicsComponent::update(double seconds)
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	auto modelTransform = transformComponent->getModelTransform();

	if (!transformComponent->isOnGround()) {
		velocity -= glm::vec3(0, gravity * seconds, 0);
		std::cout << "Not on ground" << std::endl;
	}
	else std::cout << "On ground" << std::endl;

	auto moveDistance = velocity * (float)seconds;

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

void PhysicsComponent::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}