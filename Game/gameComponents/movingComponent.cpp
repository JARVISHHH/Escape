#include "movingComponent.h"

MovingComponent::MovingComponent(glm::vec3 direction, float speed)
	:GameComponent("moving"), direction(direction), speed(speed)
{
}

void MovingComponent::start()
{
	transform = gameObject->getComponent<TransformComponent>("transform");
}

void MovingComponent::update(double seconds)
{
	auto curPos = transform->getModelTransform()->getPos();
	transform->getModelTransform()->setPos(curPos + (float)seconds * speed * direction);
}