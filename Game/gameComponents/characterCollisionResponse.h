#pragma once

#include <Game/components.h>
#include "Engine/Game/components/collisionResponseComponent.h"

class CharacterCollisionResponse : public CollisionResponseComponent {
public:
	void start();
	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;

private:
	std::shared_ptr<HealthComponent> healthComponent;
};