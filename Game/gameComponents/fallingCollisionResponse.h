#pragma once

#include "Engine/Game/components/collisionResponseComponent.h"

class FallingCollisionResponse : public CollisionResponseComponent {
public:
	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;
};