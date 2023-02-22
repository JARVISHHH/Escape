#pragma once

#include "Engine/Game/components/collisionResponseComponent.h"

class CharacterCollisionResponse : public CollisionResponseComponent {
public:
	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;
};