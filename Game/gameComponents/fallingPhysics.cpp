#include "fallingPhysics.h"
#include <Engine/Game/components/transformComponent.h>

FallingPhysics::FallingPhysics()
	: PhysicsComponent()
{
	gravity = 0;
	velocity = glm::vec3(0, -1, 0);
}

void FallingPhysics::update(double seconds)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	auto modelTransform = transformComponent->getModelTransform();

	auto moveDistance = velocity * (float)seconds;
	modelTransform->translate(glm::vec3(0, moveDistance.y, 0));

	if (modelTransform->getPos().y < -10) {
		gameObject->setActiveStatus(false);
		gameObject->getGameWorld()->removeGameObject(gameObject);
	}
}