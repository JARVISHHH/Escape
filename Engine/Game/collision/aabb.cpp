#include "aabb.h"
#include <Engine/Game/components/transformComponent.h>

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

	center = (maxPoint + minPoint) / 2.0f;
}

AABB::AABB(glm::vec4 maxPoint, glm::vec4 minPoint) {
	this->maxPoint = maxPoint;
	this->minPoint = minPoint;
	center = (maxPoint + minPoint) / 2.0f;
}

float AABB::checkCollision(std::shared_ptr<Ray> ray)
{
	float res = -1.0f;

	for (int i = 0; i < 3; i++) {
		float t = (maxPoint[i] - ray->origin[i]) / ray->direction[i];
		if (t >= 0) {
			float otherAxes[2];
			bool collide = true;
			for (int j = 0; j < 2; j++) {
				otherAxes[j] = ray->origin[(i + j) % 3] + t * ray->direction[(i + j) % 3];
				if (otherAxes[j] <= maxPoint[(i + j) % 3] && otherAxes[j] >= minPoint[(i + j) % 3]) res = res == -1.0f ? t : std::min(res, t);
			}
		}
		t = (minPoint[i] - ray->origin[i]) / ray->direction[i];
		if (t >= 0) {
			float otherAxes[2];
			bool collide = true;
			for (int j = 0; j < 2; j++) {
				otherAxes[j] = ray->origin[(i + j) % 3] + t * ray->direction[(i + j) % 3];
				if (otherAxes[j] <= maxPoint[(i + j) % 3] && otherAxes[j] >= minPoint[(i + j) % 3]) res = res == -1.0f ? t : std::min(res, t);
			}
		}
	}

	return res;
}

std::shared_ptr<CollisionInfo> AABB::getCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray)
{
	auto triangleTransformMatrix = environmentComponent->getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();
	return triangle->intersect(transformMatrix, triangleTransformMatrix, ray);
}