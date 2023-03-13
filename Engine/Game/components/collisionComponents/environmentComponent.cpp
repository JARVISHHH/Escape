#include "environmentComponent.h"
#include <Engine/Game/components/transformComponent.h>

EnvironmentComponent::EnvironmentComponent(std::vector<std::shared_ptr<Triangle>> triangles)
	:GameComponent("environment"), triangles(triangles)
{
}

std::shared_ptr<CollisionInfo> EnvironmentComponent::getClosestCollision(glm::mat4x4 transformMatrix, std::shared_ptr<Ray> ray)
{
	auto res = std::make_shared<CollisionInfo>();
	transformMatrix = transformMatrix * gameObject->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();

	for (auto triangle : triangles) {
		auto thisCollision = triangle->intersect(transformMatrix, ray);
		if (thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	}

	return res;
}