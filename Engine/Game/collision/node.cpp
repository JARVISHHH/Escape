#include "node.h"

std::shared_ptr<CollisionInfo> Node::getCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray, std::shared_ptr<Ray> sphereSpaceRay)
{
	if (boundingBox->checkCollision(ray) < 0) return nullptr;
	auto res = std::make_shared<CollisionInfo>();
	if (left != nullptr) {
		std::shared_ptr<CollisionInfo> thisCollision = left->getCollision(transformMatrix, ray, sphereSpaceRay);
		if (thisCollision != nullptr && thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) 
			res = thisCollision;
	}
	if (right != nullptr) {
		std::shared_ptr<CollisionInfo> thisCollision = right->getCollision(transformMatrix, ray, sphereSpaceRay);
		if (thisCollision != nullptr && thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) 
			res = thisCollision;
	}
	for (const auto& aabb : aabbs) {
		if (aabb->checkCollision(ray) < 0) continue;
		auto thisCollision = aabb->getCollision(transformMatrix, sphereSpaceRay);
		if (thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	}

	return res;
}