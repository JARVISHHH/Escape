#include "scopeCondition.h"

ScopeCondition::ScopeCondition(std::string targetTag, float maxDistance, std::shared_ptr<NavMesh> navMesh)
	:targetTag(targetTag), maxDistance(maxDistance), navMesh(navMesh)
{

}

void ScopeCondition::start()
{
	transform = behaviorComponent->getGameObject()->getComponent<TransformComponent>("transform");
	auto targetObjects = behaviorComponent->getGameObject()->getGameWorld()->getGameObjects(targetTag);
	if (targetObjects.size() > 0) targetTransform = targetObjects[0]->getComponent<TransformComponent>("tranform");
}

bool ScopeCondition::checkCondition(double seconds)
{
	auto sourcePos = navMesh->rayCast(transform->getModelTransform()->getPos(), glm::vec3(0, -1, 0));
	auto targetPos = navMesh->rayCast(targetTransform->getModelTransform()->getPos(), glm::vec3(0, -1, 0));
	float curDistance = glm::length(sourcePos - targetPos);
	if (curDistance <= maxDistance) return true;
	else return false;
}
