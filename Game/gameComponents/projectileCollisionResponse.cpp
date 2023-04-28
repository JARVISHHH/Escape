#include "projectileCollisionResponse.h"

ProjectileCollisionResponse::ProjectileCollisionResponse()
	: CollisionResponseComponent()
{
}

ProjectileCollisionResponse::ProjectileCollisionResponse(bool isStatic)
	: CollisionResponseComponent(isStatic)
{
}

void ProjectileCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	gameObject->setActiveStatus(false);
	std::cout << std::endl << "collide with: " << otherComponent->getGameObject()->getTag() << std::endl;
}

void ProjectileCollisionResponse::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (collisions.size() <= 0) return;
	gameObject->setActiveStatus(false);
	std::cout << std::endl << "collide with environment" << std::endl;
}