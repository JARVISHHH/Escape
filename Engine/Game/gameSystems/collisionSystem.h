#pragma once

#include "Engine/Game/gameSystem.h"

class CollisionComponent;
class CollisionResponseComponent;

class CollisionSystem : public GameSystem {
public:
	CollisionSystem();

	void update(double seconds);
	void doCollision();
	void notifyCollision(int index1, int index2, glm::vec3 mtv);
	void addGameObject(std::shared_ptr<GameObject> gameObject);

protected:
	typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> componentPair;
	std::vector<componentPair> componentPairs;
};