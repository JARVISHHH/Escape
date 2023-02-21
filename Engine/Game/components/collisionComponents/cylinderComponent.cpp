#include "cylinderComponent.h"
#include <Engine/Game/components/transformComponent.h>

CylinderComponent::CylinderComponent()
{
	self = std::shared_ptr<CylinderComponent>(this);
}

bool CylinderComponent::checkCollision(std::shared_ptr<CollisionComponent> component)
{
	return component->checkCollision(self);
}

bool CylinderComponent::checkCollision(std::shared_ptr<CylinderComponent> component) {
	auto transformComponentA = gameObject->getComponent<TransformComponent>("transform");
	auto transformComponentB = component->gameObject->getComponent<TransformComponent>("transform");
	auto modelMatrixA = transformComponentA->getModelTransform()->getModelMatrix(), modelMatrixB = transformComponentB->getModelTransform()->getModelMatrix();
	auto circleCenterA = modelMatrixA * glm::vec4(0, 0, 0, 1), circleCenterB = modelMatrixB * glm::vec4(0, 0, 0, 1);
	float radiusA = transformComponentA->getModelTransform()->getScale().x * 0.5, radiusB = transformComponentB->getModelTransform()->getScale().x * 0.5;
	if (pow(circleCenterA.x - circleCenterB.x, 2) + pow(circleCenterA.z - circleCenterB.z, 2) >= pow(radiusA + radiusB, 2)) return false;
	float topA = (modelMatrixA * glm::vec4(0, 0, 0.5, 1)).y, bottomA = (modelMatrixA * glm::vec4(0, 0, -0.5, 1)).y;
	float topB = (modelMatrixB * glm::vec4(0, 0, 0.5, 1)).y, bottomB = (modelMatrixB * glm::vec4(0, 0, -0.5, 1)).y;
	if (topA < bottomA) std::swap(topA, bottomA);
	if (topB < bottomB) std::swap(topB, bottomB);
	if (bottomA > topB || bottomB > topA) return false;
	return true;
}