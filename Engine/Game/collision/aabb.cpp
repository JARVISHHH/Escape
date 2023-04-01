#include "aabb.h"
#include <Engine/Game/components/transformComponent.h>

AABB::AABB(std::shared_ptr<EnvironmentComponent> environmentComponent)
{
	this->environmentComponent = environmentComponent;
	auto triangleTransformMatrix = environmentComponent->getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();

	maxPoint = triangleTransformMatrix * environmentComponent->getTriangles()[0]->getV()[0]->getPosition();
	minPoint = maxPoint;

	for (const auto& triangle : environmentComponent->getTriangles()) {
		for (const auto& vertex : triangle->getV()) {
			auto vertexPos = triangleTransformMatrix * vertex->getPosition();
			for (int i = 0; i < 3; i++) {
				maxPoint[i] = std::max(maxPoint[i], vertexPos[i]);
				minPoint[i] = std::min(minPoint[i], vertexPos[i]);
			}
		}
	}

	center = (maxPoint + minPoint) / 2.0f;
}

AABB::AABB(glm::vec4 maxPoint, glm::vec4 minPoint) {
	this->maxPoint = maxPoint;
	this->minPoint = minPoint;
	center = (maxPoint + minPoint) / 2.0f;
}