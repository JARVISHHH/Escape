#include "aabb.h"
#include <Engine/Game/components/transformComponent.h>

AABB::AABB()
{
}

AABB::AABB::AABB(std::shared_ptr<EnvironmentComponent> environmentComponent, std::shared_ptr<Triangle> triangle)
{
	this->triangle = triangle;
	this->environmentComponent = environmentComponent;
	auto triangleTransformMatrix = environmentComponent->getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();

	maxPoint = triangleTransformMatrix * environmentComponent->getTriangles()[0]->getV()[0]->getPosition();
	minPoint = maxPoint;

	for (const auto& vertex : triangle->getV()) {
		auto vertexPos = triangleTransformMatrix * vertex->getPosition();
		for (int i = 0; i < 3; i++) {
			maxPoint[i] = std::max(maxPoint[i], vertexPos[i]);
			minPoint[i] = std::min(minPoint[i], vertexPos[i]);
		}
	}

	//std::cout << "Min: " << minPoint[0] << " " << minPoint[1] << " " << minPoint[2] << std::endl;
	//std::cout << "Max: " << maxPoint[0] << " " << maxPoint[1] << " " << maxPoint[2] << std::endl;

	center = (maxPoint + minPoint) / 2.0f;
}

AABB::AABB(glm::vec4 maxPoint, glm::vec4 minPoint) {
	this->maxPoint = maxPoint;
	this->minPoint = minPoint;
	center = (maxPoint + minPoint) / 2.0f;
}

bool AABB::checkCollision(std::shared_ptr<AABB> movingAABB)
{
	if (minPoint[0] > movingAABB->maxPoint[0] || maxPoint[0] < movingAABB->minPoint[0]) return false;
	if (minPoint[1] > movingAABB->maxPoint[1] || maxPoint[1] < movingAABB->minPoint[1]) return false;
	if (minPoint[2] > movingAABB->maxPoint[2] || maxPoint[2] < movingAABB->minPoint[2]) return false;
	return true;
}

std::shared_ptr<CollisionInfo> AABB::getCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray)
{
	auto triangleTransformMatrix = environmentComponent->getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();
	return triangle->intersect(transformMatrix, triangleTransformMatrix, ray);
}