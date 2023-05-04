#include "environmentComponent.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/collision/aabb.h>

EnvironmentComponent::EnvironmentComponent(std::vector<std::shared_ptr<Triangle>> triangles)
	:GameComponent("environment"), triangles(triangles)
{
}

EnvironmentComponent::~EnvironmentComponent()
{
	//std::cout << "environment component delete" << std::endl;
}

void EnvironmentComponent::buildBoundingBox()
{
	aabbs.clear();
	aabbs.reserve(triangles.size());
	for (const auto& triangle : triangles) {
		aabbs.push_back(std::make_shared<AABB>(shared_from_this(), triangle));
	}
	//glm::vec3 maxPoint = aabbs[0]->getMaxPoint(), minPoint = aabbs[0]->getMinPoint();
	//for (auto aabb : aabbs) {
	//	for (int i = 0; i < 3; i++) {
	//		maxPoint[i] = std::max(maxPoint[i], aabb->getMaxPoint()[i]);
	//		minPoint[i] = std::min(minPoint[i], aabb->getMinPoint()[i]);
	//	}
	//}
	//std::cout << "maxPoint: " << maxPoint[0] << " " << maxPoint[1] << " " << maxPoint[2] << std::endl;
	//std::cout << "minPoint: " << minPoint[0] << " " << minPoint[1] << " " << minPoint[2] << std::endl;
}

std::shared_ptr<CollisionInfo> EnvironmentComponent::getClosestCollision(glm::mat4x4 inverseTransformMatrix, std::shared_ptr<Ray> ray)
{
	auto res = std::make_shared<CollisionInfo>();
	auto triangleTransformMatrix = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();

	for (auto triangle : triangles) {
		auto thisCollision = triangle->intersect(inverseTransformMatrix, triangleTransformMatrix, ray);
		if (thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	}

	res->environmentComponent = shared_from_this();

	return res;
}

const std::vector<std::shared_ptr<Triangle>>& EnvironmentComponent::getTriangles()
{
	return triangles;
}