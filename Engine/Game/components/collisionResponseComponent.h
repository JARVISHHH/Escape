#pragma once

#include "Engine/Game/gameComponent.h"

class CollisionResponseComponent : public GameComponent
{
public:
	CollisionResponseComponent();
	CollisionResponseComponent(bool isStatic);

	void doCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);
	void doCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos);
	virtual void resolveCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);
	virtual void resolveCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos);
	virtual void responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv);
	virtual void responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos);

private:
	bool isStatic = false;
};