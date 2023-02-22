#pragma once

#include "Engine/Game/gameComponent.h"

class CollisionResponseComponent : public GameComponent
{
public:
	CollisionResponseComponent();
	CollisionResponseComponent(bool isStatic);

	void doCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);
	virtual void resolveCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);
	virtual void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);

private:
	bool isStatic = false;
};