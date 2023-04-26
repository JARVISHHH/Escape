#pragma once

#include "Engine/Game/gameSystem.h"
#include <Engine/Game/collision/bvh.h>
#include <Engine/Game/collision/hierarchicalGrid.h>

class CollisionComponent;
class CollisionResponseComponent;
class EnvironmentComponent;

class CollisionSystem : public GameSystem {
public:
	CollisionSystem(std::shared_ptr<GameWorld> gameWorld, int level);

	void update(double seconds);
	void doCollision();
	void notifyEnvironmentCollision(int index, std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos);
	void notifyCollision(int index1, int index2, glm::vec3 mtv);

	void addGameObject(std::shared_ptr<GameObject> gameObject);
	void addEnvironmentObject(std::shared_ptr<GameObject> gameEnvironment);

	void updateEntityComponentPairs();

	void buildBVH();
	void buildHG();

protected:
	typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;
	std::vector<std::shared_ptr<entityComponentPair>> entityComponentPairs;
	std::vector<std::shared_ptr<EnvironmentComponent>> environmentComponents;
	std::shared_ptr<BVH> bvh;
	std::shared_ptr<HierarchicalGrid> hierarchicalGrid;
};