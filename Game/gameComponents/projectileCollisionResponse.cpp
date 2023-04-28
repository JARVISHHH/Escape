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

}