#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>
#include <Engine/Game/components/transformComponent.h>

CollisionSystem::CollisionSystem(std::shared_ptr<GameWorld> gameWorld, int level)
	: GameSystem("collision"), hierarchicalGrid(std::make_shared<HierarchicalGrid>(level, gameWorld->getAABB()))
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
	updateEntityComponentPairs();

	bool doAccelerate = true;

	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i]->first->getGameObject()->getComponent<TransformComponent>("transform")->updateRay();
	}

	// Update movable game objects
	if (!doAccelerate) {
		int sum = 0;
		for (int i = 0; i < entityComponentPairs.size(); i++)
			for (int j = 0; j < i; j++) {
				sum++;
				auto collisionComponent1 = entityComponentPairs[i]->first;
				auto collisionComponent2 = entityComponentPairs[j]->first;
				glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
				if (glm::length(mtv) == 0) continue;  // No collision
				notifyCollision(i, j, mtv);
			}
		//std::cout << "collision: " << sum << std::endl;
	}
	else {
		for (int i = 0; i < entityComponentPairs.size(); i++) {
			hierarchicalGrid->update(entityComponentPairs[i]);
		}
		hierarchicalGrid->collide();
	}

	// Check collision between environments
	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i]->first->getGameObject()->getComponent<TransformComponent>("transform")->updateRayEnd();
		std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> collisionRes;
		if(!doAccelerate) collisionRes = entityComponentPairs[i]->first->ellipsoidTriangleCollision(environmentComponents);
		else collisionRes = entityComponentPairs[i]->first->ellipsoidTriangleCollision(bvh);
		//auto collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(environmentComponents);
		//auto collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(bvh);
		notifyEnvironmentCollision(i, collisionRes.first, collisionRes.second);
	}

	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i]->first->getGameObject()->getComponent<TransformComponent>("transform")->updateRayEnd();
	}
}

void CollisionSystem::notifyEnvironmentCollision(int index, std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (entityComponentPairs[index]->second != nullptr)
		entityComponentPairs[index]->second->doCollision(collisions, curPos);
}

void CollisionSystem::notifyCollision(int index1, int index2, glm::vec3 mtv)
{
	if(entityComponentPairs[index1]->second != nullptr)
		entityComponentPairs[index1]->second->doCollision(entityComponentPairs[index2]->second, mtv);
	if(entityComponentPairs[index2]->second != nullptr)
		entityComponentPairs[index2]->second->doCollision(entityComponentPairs[index1]->second, -mtv);
}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	auto collisionComponents = gameObject->getComponents<CollisionComponent>("collision");
	auto collisionResponseComponent = gameObject->getComponent<CollisionResponseComponent>("collisionResponse");

	for (auto collisionComponent : collisionComponents) {
		auto entity = std::make_shared<entityComponentPair>(collisionComponent, collisionResponseComponent);
		entityComponentPairs.push_back(entity);
		hierarchicalGrid->insert(1, entity, collisionComponent->getAABB());
	}
}

void CollisionSystem::addEnvironmentObject(std::shared_ptr<GameObject> gameEnvironment)
{
	auto environmentComponent = gameEnvironment->getComponent<EnvironmentComponent>("environment");
	if (environmentComponent == nullptr) return;
	environmentComponent->buildBoundingBox();
	environmentComponents.push_back(environmentComponent);
}

void CollisionSystem::updateEntityComponentPairs()
{
	for (int i = entityComponentPairs.size() - 1; i >= 0; i--)
		if (!entityComponentPairs[i]->first->getGameObject()->getActiveStatus())
			entityComponentPairs.erase(entityComponentPairs.begin() + i);
}

void CollisionSystem::buildBVH()
{
	bvh = std::make_shared<BVH>(environmentComponents);
	bvh->buildTree();
}

void CollisionSystem::buildHG()
{
	for (int i = 0; i < entityComponentPairs.size(); i++)
		hierarchicalGrid->insert(1, entityComponentPairs[i], entityComponentPairs[i]->first->getAABB());
	//hierarchicalGrid->print();
}
