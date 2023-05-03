#pragma once

#include <unordered_set>
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
	~GridNode() {
		//std::cout << "grid node delete" << std::endl;
	}

	void collide(std::shared_ptr<GridNode> node) {
		for(auto i = entityComponentPairs.begin(); i != entityComponentPairs.end(); i++) {
		//for (int i = 0; i < entityComponentPairs.size(); i++) {
			auto endPos = node->entityComponentPairs.end();
			if (node == shared_from_this()) endPos = i;
			for (auto j = node->entityComponentPairs.begin(); j != endPos; j++) {
				sum++;
				auto collisionComponent1 = (*i)->first;
				auto collisionComponent2 = (*j)->first;
				glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
				if (glm::length(mtv) == 0) continue;  // No collision
				
				// notify
				if ((*i)->second != nullptr)
					(*i)->second->doCollision((*j)->second, mtv);
				if ((*j)->second != nullptr)
					(*j)->second->doCollision((*i)->second, -mtv);
			}
		}
	}

	int id;
	std::shared_ptr<AABB> aabb;
	std::unordered_set<std::shared_ptr<entityComponentPair>> entityComponentPairs;
};