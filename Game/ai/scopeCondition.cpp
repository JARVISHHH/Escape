#include "scopeCondition.h"

ScopeCondition::ScopeCondition(std::string targetTag, float maxDistance, std::shared_ptr<NavMesh> navMesh)
	:targetTag(targetTag), maxDistance(maxDistance), navMesh(navMesh)
{

}

void ScopeCondition::start()
{
	transform = behaviorComponent->getGameObject()->getComponent<TransformComponent>("transform");
	auto targetObjects = behaviorComponent->getGameObject()->getGameWorld()->getGameObjects(targetTag);
	if (targetObjects.size() > 0) targetTransform = targetObjects[0]->getComponent<TransformComponent>("transform");
	std::cout << behaviorComponent->blackBoard["test2"] << std::endl;
}

bool ScopeCondition::checkCondition(double seconds)
{
	//auto sourceT = navMesh->rayCast(transform->getModelTransform()->getPos(), glm::vec3(0, -1, 0));
	//auto targetT = navMesh->rayCast(targetTransform->getModelTransform()->getPos(), glm::vec3(0, -1, 0));
	auto sourcePos = transform->getModelTransform()->getPos();
	auto targetPos = targetTransform->getModelTransform()->getPos();
	float curDistance = glm::length(sourcePos - targetPos);
	std::cout << curDistance << std::endl;
	if (curDistance <= maxDistance) return false;
	else return true;
}
