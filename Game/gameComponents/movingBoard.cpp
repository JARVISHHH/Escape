#include "movingBoard.h"
#include <Engine/Game/components/transformComponent.h>

MovingBoard::MovingBoard(float velocity, glm::vec3 startPos, glm::vec3 endPos)
	: PhysicsComponent(),
	startPos(startPos), endPos(endPos)
{
	gravity = 0;
	this->velocity = velocity * glm::normalize(endPos - startPos);
}

void MovingBoard::update(double seconds)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	auto modelTransform = transformComponent->getModelTransform();

	auto moveDistance = velocity * (float)seconds;
	modelTransform->translate(moveDistance);

	if (glm::dot(endPos - startPos, moveDistance) > 0) {
		if (glm::dot(endPos - modelTransform->getPos(), endPos - startPos) <= 0) {
			modelTransform->setPos(endPos);
			velocity = -velocity;
		}
	}
	else {
		if (glm::dot(startPos - modelTransform->getPos(), startPos - endPos) <= 0) {
			modelTransform->setPos(startPos);
			velocity = -velocity;
		}
	}
}