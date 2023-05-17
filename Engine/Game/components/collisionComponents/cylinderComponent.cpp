#include "cylinderComponent.h"
#include <Engine/Game/components/transformComponent.h>

CylinderComponent::CylinderComponent()
	:CollisionComponent()
{
}

CylinderComponent::CylinderComponent(std::shared_ptr<ModelTransform> modelTransform)
	:CollisionComponent(modelTransform)
{
}

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
	//auto transformComponentA = getGameObject()->getComponent<TransformComponent>("transform");
	//auto transformComponentB = component->getGameObject()->getComponent<TransformComponent>("transform");
	auto transformA = getTransform(), transformB = component->getTransform();
	auto transformMatrixA = getTransformMatrix(), transformMatrixB = component->getTransformMatrix();
	auto circleCenterA = transformMatrixA * glm::vec4(0, 0, 0, 1), circleCenterB = transformMatrixB * glm::vec4(0, 0, 0, 1);
	float radiusA = transformA->getScale().x * 0.5, radiusB = transformB->getScale().x * 0.5;
	if (pow(circleCenterA.x - circleCenterB.x, 2) + pow(circleCenterA.z - circleCenterB.z, 2) >= pow(radiusA + radiusB, 2)) return glm::vec3(0, 0, 0);
	float topA = (transformMatrixA * glm::vec4(0, 0.5, 0, 1)).y, bottomA = (transformMatrixA * glm::vec4(0, -0.5, 0, 1)).y;
	float topB = (transformMatrixB * glm::vec4(0, 0.5, 0, 1)).y, bottomB = (transformMatrixB * glm::vec4(0, -0.5, 0, 1)).y;
	if (topA < bottomA) std::swap(topA, bottomA);
	if (topB < bottomB) std::swap(topB, bottomB);
	if (bottomA > topB || bottomB > topA) return glm::vec3(0, 0, 0);

	auto lineMTV = calculateLineMTV(topA, bottomA, topB, bottomB), circleMTV = calculateCircleMtv(circleCenterA, circleCenterB, radiusA, radiusB);
	auto MTV = glm::length(lineMTV) < glm::length(circleMTV) ? lineMTV : circleMTV;
	return MTV;
}

// Transform collision component from world space to sphere space
// pos -> world space position
glm::mat4x4 CylinderComponent::getInverseTransformMatrix(bool curPos, glm::vec3 pos)
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	std::shared_ptr<ModelTransform> transform = std::make_shared<ModelTransform>();
	auto transformMatrix = transformComponent->getModelTransform()->getModelMatrix() * modelTransform->getModelMatrix();
	if (!curPos) transform->translate(pos - glm::vec3(transformMatrix * glm::vec4(0, 0, 0, 1)));
	glm::mat4x4 inverseTransformMatrix = glm::inverse(transform->getModelMatrix() * transformMatrix);
	inverseTransformMatrix = glm::mat4x4(2, 0, 0, 0,
								  0, 2, 0, 0,
								  0, 0, 2, 0,
								  0, 0, 0, 1) * inverseTransformMatrix;

	return inverseTransformMatrix;
}

std::shared_ptr<AABB> CylinderComponent::getAABB()
{
	std::vector<glm::vec4> points(8);
	points[0] = { 0.5, 0.5, 0.5, 1 };
	points[1] = { -0.5, 0.5, 0.5, 1 };
	points[2] = { -0.5, 0.5, -0.5, 1 };
	points[3] = { 0.5, 0.5, -0.5, 1 };
	points[4] = { 0.5, -0.5, 0.5, 1 };
	points[5] = { -0.5, -0.5, 0.5, 1 };
	points[6] = { -0.5, -0.5, -0.5, 1 };
	points[7] = { 0.5, -0.5, -0.5, 1 };

	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	glm::mat4x4 transformMatrix = transformComponent->getModelTransform()->getModelMatrix() * modelTransform->getModelMatrix();
	for (int i = 0; i < 8; i++) {
		points[i] = transformMatrix * points[i];
	}

	auto maxPoint = points[0], minPoint = points[0];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], points[i][j]);
			minPoint[j] = std::min(minPoint[j], points[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

// ray indicates the position of the game objects
std::shared_ptr<AABB> CylinderComponent::getAABB(std::shared_ptr<Ray> ray)
{
	std::vector<glm::vec4> points(8);
	std::vector<glm::vec4> worldPoints(16);
	points[0] = { 0.5, 0.5, 0.5, 1 };
	points[1] = { -0.5, 0.5, 0.5, 1 };
	points[2] = { -0.5, 0.5, -0.5, 1 };
	points[3] = { 0.5, 0.5, -0.5, 1 };
	points[4] = { 0.5, -0.5, 0.5, 1 };
	points[5] = { -0.5, -0.5, 0.5, 1 };
	points[6] = { -0.5, -0.5, -0.5, 1 };
	points[7] = { 0.5, -0.5, -0.5, 1 };

	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	glm::mat4x4 transformMatrix = transformComponent->getModelTransform()->getModelMatrix() * modelTransform->getModelMatrix();
	//auto curPos3 = transformComponent->getModelTransform()->getPos();
	//auto curPos = glm::vec4(curPos3[0], curPos3[1], curPos3[2], 1);
	auto curPos = transformMatrix * glm::vec4(0, 0, 0, 1);
	for (int i = 0; i < 8; i++) {
		worldPoints[i] = transformMatrix * points[i] + ray->endPoint - curPos;
	}
	for (int i = 8; i < 16; i++) {
		worldPoints[i] = transformMatrix * points[i - 8] + ray->origin - curPos;
	}

	auto maxPoint = worldPoints[0], minPoint = worldPoints[0];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], worldPoints[i][j]);
			minPoint[j] = std::min(minPoint[j], worldPoints[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

void CylinderComponent::updateOnGround()
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	glm::mat4x4 transformMatrix = transformComponent->getModelTransform()->getModelMatrix() * modelTransform->getModelMatrix();
	glm::vec3 source = transformMatrix * glm::vec4(0, 0, 0, 1);
	glm::vec3 target = source + glm::vec3(0, -1, 0);
	//std::cout << "source: " << source[0] << " " << source[1] << " " << source[2] << std::endl;
	//std::cout << "target: " << target[0] << " " << target[1] << " " << target[2] << std::endl;
	auto collisionInfo = collisionSystem.lock()->environmentRayCast(shared_from_this(), source, target, getInverseTransformMatrix());
	//std::cout << collisionInfo->t << std::endl;
	if(std::abs(collisionInfo->t) > 0.01) transformComponent->setOnGround(false);
	else transformComponent->setOnGround(true);
}