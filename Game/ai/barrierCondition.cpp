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
	auto modelTransform = std::make_shared<ModelTransform>();
	modelTransform->scale(0.05);
	modelTransform->translate(source);
	auto collisionInfo = collisionSystem->environmentRayCast(CylinderComponent::getAABB(modelTransform, std::make_shared<Ray>(source, target)), source, target, glm::inverse(modelTransform->getModelMatrix()));
	//std::cout << "source " << source[0] << " " << source[1] << " " << source[2] << std::endl;
	//std::cout << "target " << target[0] << " " << target[1] << " " << target[2] << std::endl;
	//std::cout << collisionInfo->t << std::endl;
	if (collisionInfo->t <= 0 || glm::length(target - source) < collisionInfo->t) return false;
	//std::cout << "barrier" << std::endl;
	return true;
}