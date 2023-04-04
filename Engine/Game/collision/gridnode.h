#pragma once

#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"

typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;

extern int sum;

class GridNode : public std::enable_shared_from_this<GridNode> {
public:
	GridNode(int id) { 
		this->id = id; 
		aabb = std::make_shared<AABB>();
	}

	void collide(std::shared_ptr<GridNode> node) {
		for (int i = 0; i < entityComponentPairs.size(); i++) {
			int endPos = node->entityComponentPairs.size();
			if (node == shared_from_this()) endPos = i;
			for (int j = 0; j < endPos; j++) {
				sum++;
				auto collisionComponent1 = entityComponentPairs[i].first;
				auto collisionComponent2 = node->entityComponentPairs[j].first;
				glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
				if (glm::length(mtv) == 0) continue;  // No collision
				
				// notify
				if (entityComponentPairs[i].second != nullptr)
					entityComponentPairs[i].second->doCollision(node->entityComponentPairs[j].second, mtv);
				if (node->entityComponentPairs[j].second != nullptr)
					node->entityComponentPairs[j].second->doCollision(entityComponentPairs[i].second, -mtv);
			}
		}
	}

	int id;
	std::shared_ptr<AABB> aabb;
	std::vector<entityComponentPair> entityComponentPairs;
};