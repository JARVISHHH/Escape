#include "barrierCondition.h"

BarrierCondition::BarrierCondition(std::string targetTag)
	:targetTag(targetTag)
{
}

void BarrierCondition::start()
{
	collisionSystem = behaviorComponent->getGameObject()->getGameWorld()->getGameSystem<CollisionSystem>("collision");

	transform = behaviorComponent->getGameObject()->getComponent<TransformComponent>("transform");

	auto targetObject = behaviorComponent->getGameObject()->getGameWorld()->getGameObject(targetTag);
	if (targetObject != nullptr) targetTransform = targetObject->getComponent<TransformComponent>("transform");
}

bool BarrierCondition::checkCondition(double seconds)
{
	auto source = transform->getModelTransform()->getPos();
	auto target = targetTransform->getModelTransform()->getPos();
	auto collisionInfo = collisionSystem->environmentRayCast(source, target);
	if (collisionInfo->t <= 0 || glm::length(target - source) < collisionInfo->t) return false;
	return true;
}