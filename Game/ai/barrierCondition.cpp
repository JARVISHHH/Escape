#include "barrierCondition.h"

BarrierCondition::BarrierCondition(std::string targetTag)
	:targetTag(targetTag)
{
}

void BarrierCondition::start()
{
	collisionSystem = getBehaviorComponent()->getGameObject()->getGameWorld()->getGameSystem<CollisionSystem>("collision");

	transform = getBehaviorComponent()->getGameObject()->getComponent<TransformComponent>("transform");

	auto targetObject = getBehaviorComponent()->getGameObject()->getGameWorld()->getGameObject(targetTag);
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