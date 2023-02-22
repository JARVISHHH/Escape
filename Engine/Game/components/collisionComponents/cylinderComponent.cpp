#include "cylinderComponent.h"
#include <Engine/Game/components/transformComponent.h>

glm::vec3 CylinderComponent::checkCollision(std::shared_ptr<CollisionComponent> component)
{
	return -component->checkCollision(std::static_pointer_cast<CylinderComponent>((shared_from_this())));
}

glm::vec3 calculateLineMTV(float topA, float bottomA, float topB, float bottomB) {
	float aUp = topB - bottomA, aDown = topA - bottomB;
	if (aUp < aDown) return glm::vec3(0, aUp, 0);
	else return glm::vec3(0, -aDown, 0);
}

glm::vec3 calculateCircleMtv(glm::vec4 circleCenterA, glm::vec4 circleCenterB, float radiusA, float radiusB) {
	float circleDistance = std::sqrt(pow(circleCenterA.x - circleCenterB.x, 2) + pow(circleCenterA.z - circleCenterB.z, 2));
	return (circleCenterA - circleCenterB) * (radiusA + radiusB - circleDistance);
}

glm::vec3 CylinderComponent::checkCollision(std::shared_ptr<CylinderComponent> component) {
	auto transformComponentA = gameObject->getComponent<TransformComponent>("transform");
	auto transformComponentB = component->gameObject->getComponent<TransformComponent>("transform");
	auto modelMatrixA = transformComponentA->getModelTransform()->getModelMatrix(), modelMatrixB = transformComponentB->getModelTransform()->getModelMatrix();
	auto circleCenterA = modelMatrixA * glm::vec4(0, 0, 0, 1), circleCenterB = modelMatrixB * glm::vec4(0, 0, 0, 1);
	float radiusA = transformComponentA->getModelTransform()->getScale().x * 0.5, radiusB = transformComponentB->getModelTransform()->getScale().x * 0.5;
	if (pow(circleCenterA.x - circleCenterB.x, 2) + pow(circleCenterA.z - circleCenterB.z, 2) >= pow(radiusA + radiusB, 2)) return glm::vec3(0, 0, 0);
	float topA = (modelMatrixA * glm::vec4(0, 0.5, 0, 1)).y, bottomA = (modelMatrixA * glm::vec4(0, -0.5, 0, 1)).y;
	float topB = (modelMatrixB * glm::vec4(0, 0.5, 0, 1)).y, bottomB = (modelMatrixB * glm::vec4(0, -0.5, 0, 1)).y;
	if (topA < bottomA) std::swap(topA, bottomA);
	if (topB < bottomB) std::swap(topB, bottomB);
	if (bottomA > topB || bottomB > topA) return glm::vec3(0, 0, 0);

	auto lineMTV = calculateLineMTV(topA, bottomA, topB, bottomB), circleMTV = calculateCircleMtv(circleCenterA, circleCenterB, radiusA, radiusB);
	auto MTV = glm::length(lineMTV) < glm::length(circleMTV) ? lineMTV : circleMTV;
	return MTV;
}

