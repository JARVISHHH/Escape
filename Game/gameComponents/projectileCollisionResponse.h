#pragma once

#include <Game/components.h>
#include "Engine/Game/components/collisionResponseComponent.h"

class ProjectileCollisionResponse : public CollisionResponseComponent {
public:
	ProjectileCollisionResponse();
	ProjectileCollisionResponse(bool isStatic);
	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;
	void responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos) override;

private:

};