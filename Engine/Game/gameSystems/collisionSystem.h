#pragma once

#include "Engine/Game/gameSystem.h"
#include <Engine/Game/collision/bvh.h>

#include <unordered_set>

class CollisionComponent;
class CollisionResponseComponent;
class EnvironmentComponent;

class HierarchicalGrid;

class CollisionSystem : public GameSystem {
public:
	CollisionSystem(std::shared_ptr<GameWorld> gameWorld, int level);

	void update(double seconds);
	void doCollision();
	void notifyEnvironmentCollision(std::string layer, int index, std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos);
	void notifyCollision(std::string layer1, std::string layer2, int index1, int index2, glm::vec3 mtv);

	void addLayer(std::string layer);
	void addLayerCollision(std::string layer1, std::string layer2);
	void deleteLayerCollision(std::string layer1, std::string layer2);

	void addGameObject(std::shared_ptr<GameObject> gameObject, std::string layer = "default");
	void addEnvironmentObject(std::shared_ptr<GameObject> gameEnvironment);

	void updateEntityComponentPairs();

	void buildBVH();
	void buildHG();

	std::shared_ptr<CollisionInfo> environmentRayCast(std::shared_ptr<CollisionComponent> collisionComponent, glm::vec3 source, glm::vec3 target, glm::mat4x4 transformMatrix = glm::mat4x4(1.0f));
	std::shared_ptr<CollisionInfo> environmentRayCast(std::shared_ptr<AABB> movingAABB, glm::vec3 source, glm::vec3 target, glm::mat4x4 transformMatrix = glm::mat4x4(1.0f));


protected:
	int HGLevel;

	typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;
	std::unordered_map<std::string, std::vector<std::shared_ptr<entityComponentPair>>> entityComponentPairs;
	std::vector<std::shared_ptr<EnvironmentComponent>> environmentComponents;

	std::unordered_map<std::string, std::vector<std::shared_ptr<entityComponentPair>>> entityWaitingList;
	//std::vector<std::shared_ptr<EnvironmentComponent>> environmentWaitingList;

	std::shared_ptr<BVH> bvh;
	std::unordered_map<std::string, std::shared_ptr<HierarchicalGrid>> hierarchicalGrids;
	std::unordered_map<std::string, std::unordered_set<std::string>> layerCollisionMatrix;
};