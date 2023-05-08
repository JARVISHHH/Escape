#pragma once

#include <Game/components.h>
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/gameSystems/particleSystem.h>

class BulletCollisionResponse : public CollisionResponseComponent {
public:
	BulletCollisionResponse();
	BulletCollisionResponse(bool isStatic);

	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;
	void responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos) override;

private:
	Particle createBloodParticle();
	Particle createGreenParticle();
};