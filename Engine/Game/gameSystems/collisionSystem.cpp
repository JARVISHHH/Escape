#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>
#include <Engine/Game/components/transformComponent.h>

CollisionSystem::CollisionSystem(std::shared_ptr<GameWorld> gameWorld, int level)
	: GameSystem("collision"), HGLevel(level)
{
	auto maxPoint = gameWorld->getAABB()->getMaxPoint(), minPoint = gameWorld->getAABB()->getMinPoint();
	std::cout << "max point: " << maxPoint[0] << " " << maxPoint[1] << " " << maxPoint[2] << std::endl;
	std::cout << "min point: " << minPoint[0] << " " << minPoint[1] << " " << minPoint[2] << std::endl;
}

void CollisionSystem::update(double seconds)
{
	doCollision();
}

void CollisionSystem::doCollision()
{
	bool doAccelerate = true;

	updateEntityComponentPairs();

	// Update movable game objects
	//if (!doAccelerate) {
	//	//int sum = 0;
	//	for (auto iter1 = entityComponentPairs.begin(); iter1 != entityComponentPairs.end(); iter1++) {
	//		auto layer1 = iter1->first;
	//		auto& pairs1 = iter1->second;
	//		for (auto iter2 = layerCollisionMatrix[layer1].begin(); iter2 != layerCollisionMatrix[layer1].end(); iter2++) {
	//			auto layer2 = *iter2;
	//			auto& pairs2 = entityComponentPairs[layer2];
	//			// Collision with other layer
	//			if (layer1.compare(layer2) != 0) {
	//				for (int i = 0; i < pairs1.size(); i++) {
	//					for (int j = 0; j < pairs2.size(); j++) {
	//						auto collisionComponent1 = pairs1[i]->first;
	//						auto collisionComponent2 = pairs2[j]->first;
	//						glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
	//						if (glm::length(mtv) == 0) continue;  // No collision
	//						notifyCollision(layer1, layer2, i, j, mtv);
	//					}
	//				}
	//			}
	//			// Self-layer collision
	//			else {
	//				for (int i = 0; i < pairs1.size(); i++) {
	//					for (int j = i + 1; j < pairs2.size(); j++) {
	//						auto collisionComponent1 = pairs1[i]->first;
	//						auto collisionComponent2 = pairs2[j]->first;
	//						glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
	//						if (glm::length(mtv) == 0) continue;  // No collision
	//						notifyCollision(layer1, layer2, i, j, mtv);
	//					}
	//				}
	//			}
	//		}
	//	}
	//	//std::cout << "collision: " << sum << std::endl;
	//}
	//else {
	//	// Update
	//	for (auto iter = entityComponentPairs.begin(); iter != entityComponentPairs.end(); iter++) {
	//		auto layer = iter->first;
	//		auto& pairs = iter->second;
	//		for (int i = 0; i < pairs.size(); i++) {
	//			hierarchicalGrids[layer]->update(pairs[i]);
	//		}
	//	}
	//	// Collide
	//	for (auto iter1 = layerCollisionMatrix.begin(); iter1 != layerCollisionMatrix.end(); iter1++) {
	//		auto layer1 = iter1->first;
	//		//std::cout << layer1 << " collide with: ";
	//		for (auto iter2 = layerCollisionMatrix[layer1].begin(); iter2 != layerCollisionMatrix[layer1].end(); iter2++) {
	//			auto layer2 = *iter2;
	//			//std::cout << layer2 << " ";
	//			HierarchicalGrid::collideAll(hierarchicalGrids[layer1], hierarchicalGrids[layer2], 1, 1);
	//		}
	//		//std::cout << std::endl;
	//	}
	//}

	//updateEntityComponentPairs();

	// Check collision between environments
	for (auto iter = entityComponentPairs.begin(); iter != entityComponentPairs.end(); iter++) {
		auto layer = iter->first;
		auto& pairs = iter->second;
		for (int i = 0; i < pairs.size(); i++)
		{
			std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> collisionRes;
			if (!doAccelerate) collisionRes = pairs[i]->first->ellipsoidTriangleCollision(environmentComponents);
			else collisionRes = pairs[i]->first->ellipsoidTriangleCollision(bvh);
			notifyEnvironmentCollision(layer, i, collisionRes.first, collisionRes.second);
		}
	}
}

void CollisionSystem::notifyEnvironmentCollision(std::string layer, int index, std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (entityComponentPairs[layer][index]->second != nullptr)
		entityComponentPairs[layer][index]->second->doCollision(collisions, curPos);
}

void CollisionSystem::notifyCollision(std::string layer1, std::string layer2, int index1, int index2, glm::vec3 mtv)
{
	if(entityComponentPairs[layer1][index1]->second != nullptr)
		entityComponentPairs[layer1][index1]->second->doCollision(entityComponentPairs[layer2][index2]->second, mtv);
	if(entityComponentPairs[layer2][index2]->second != nullptr)
		entityComponentPairs[layer2][index2]->second->doCollision(entityComponentPairs[layer1][index1]->second, -mtv);
}

void CollisionSystem::addLayer(std::string layer)
{
	//for (auto iter = entityComponentPairs.begin(); iter != entityComponentPairs.end(); iter++)
	//	std::cout << iter->first << " ";
	//std::cout << std::endl;
	if (entityComponentPairs.find(layer) != entityComponentPairs.end()) return;  // The layer already exists
	entityComponentPairs[layer] = std::vector<std::shared_ptr<entityComponentPair>>();
	hierarchicalGrids[layer] = std::make_shared<HierarchicalGrid>(HGLevel, getGameWorld()->getAABB());
	layerCollisionMatrix[layer].insert(layer);
	for (auto iter = layerCollisionMatrix.begin(); iter != layerCollisionMatrix.end(); iter++) {
		layerCollisionMatrix[layer].insert(iter->first);
	}
	//std::cout << "added " << layer << std::endl;
}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject, std::string layer)
{
	auto collisionComponents = gameObject->getComponents<CollisionComponent>("collision");
	auto collisionResponseComponent = gameObject->getComponent<CollisionResponseComponent>("collisionResponse");

	addLayer(layer);

	for (auto collisionComponent : collisionComponents) {
		auto entity = std::make_shared<entityComponentPair>(collisionComponent, collisionResponseComponent);
		entityWaitingList[layer].push_back(entity);
		//entityComponentPairs[layer].push_back(entity);
		//hierarchicalGrids[layer]->insert(1, entity, collisionComponent->getAABB());
	}
}

void CollisionSystem::addLayerCollision(std::string layer1, std::string layer2)
{
	layerCollisionMatrix[layer1].insert(layer2);
	layerCollisionMatrix[layer2].erase(layer1);
}

void CollisionSystem::deleteLayerCollision(std::string layer1, std::string layer2)
{
	layerCollisionMatrix[layer1].erase(layer2);
	layerCollisionMatrix[layer2].erase(layer1);
}

void CollisionSystem::addEnvironmentObject(std::shared_ptr<GameObject> gameEnvironment)
{
	auto environmentComponent = gameEnvironment->getComponent<EnvironmentComponent>("environment");
	if (environmentComponent == nullptr) return;
	environmentComponent->buildBoundingBox();
	//environmentWaitingList.push_back(environmentComponent);
	environmentComponents.push_back(environmentComponent);
}

void CollisionSystem::updateEntityComponentPairs()
{
	for (auto iter = entityWaitingList.begin(); iter != entityWaitingList.end();) {
		auto layer = iter->first;
		auto& pairs = iter->second;
		if (entityComponentPairs.find(layer) == entityComponentPairs.end()) entityComponentPairs[layer] = std::vector<std::shared_ptr<entityComponentPair>>();
		for (auto entityComponentPair : pairs) {
			entityComponentPairs[layer].push_back(entityComponentPair);
			hierarchicalGrids[layer]->insert(1, entityComponentPair, entityComponentPair->first->getAABB());
		}
		pairs.clear();
		iter = entityWaitingList.erase(iter);
	}
	for (auto iter = entityComponentPairs.begin(); iter != entityComponentPairs.end(); iter++) {
		auto layer = iter->first;
		auto& pairs = iter->second;
		for (int i = pairs.size() - 1; i >= 0; i--)
			if (!pairs[i]->first->getGameObject()->getActiveStatus()) {
				pairs[i]->first->gridNode.lock()->entityComponentPairs.erase(pairs[i]);
				pairs.erase(pairs.begin() + i);
			}
	}
}

void CollisionSystem::buildBVH()
{
	bvh = std::make_shared<BVH>(environmentComponents);
	bvh->buildTree();
}

void CollisionSystem::buildHG()
{
	for (auto iter = entityComponentPairs.begin(); iter != entityComponentPairs.end(); iter++) {
		auto layer = iter->first;
		auto& pairs = iter->second;
		for (int i = 0; i < pairs.size(); i++)
			hierarchicalGrids[layer]->insert(1, pairs[i], pairs[i]->first->getAABB());
	}
	
	//hierarchicalGrid->print();
}

std::shared_ptr<CollisionInfo> CollisionSystem::environmentRayCast(glm::vec3 source, glm::vec3 target)
{
	auto res = std::make_shared<CollisionInfo>();

	auto ray = std::make_shared<Ray>(source, target);
	glm::mat4x4 transformMatrix(1.0f);

	glm::vec4 maxPoint, minPoint;
	maxPoint[3] = 1, minPoint[3] = 1;
	for (int i = 0; i < 3; i++) {
		maxPoint[i] = std::max(source[i], target[i]);
		minPoint[i] = std::min(source[i], target[i]);
	}
	auto aabb = std::make_shared<AABB>(maxPoint, minPoint);
		
	auto thisCollision = bvh->getClosestCollision(transformMatrix, aabb, ray);
	if (thisCollision != nullptr && thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	
	return res;
}
