#pragma once

#include <Game/components.h>
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Game/gameComponents/gameHandlerComponent.h>

class CharacterCollisionResponse : public CollisionResponseComponent {
public:
	CharacterCollisionResponse(std::shared_ptr<GameObject> gameHandlerObject);
	void start();
	void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv) override;
	void responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos) override;

private:
	std::shared_ptr<HealthComponent> healthComponent;
	std::shared_ptr<GameObject> gameHandlerObject;
	std::shared_ptr<GameHandlerComponent> gameHandler;
};