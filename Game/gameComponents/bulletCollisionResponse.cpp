#include "bulletCollisionResponse.h"

BulletCollisionResponse::BulletCollisionResponse()
	:CollisionResponseComponent()
{
}

BulletCollisionResponse::BulletCollisionResponse(bool isStatic)
	:CollisionResponseComponent(isStatic)
{
}

void BulletCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	getGameObject()->setActiveStatus(false);
}

void BulletCollisionResponse::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (collisions.size() <= 0) return;
	getGameObject()->setActiveStatus(false);
}