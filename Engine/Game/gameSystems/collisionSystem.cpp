#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>
#include <Engine/Game/components/transformComponent.h>

CollisionSystem::CollisionSystem()
	: GameSystem("collision"), hierarchicalGrid(std::make_shared<HierarchicalGrid>(4))
{

}

void CollisionSystem::update(double seconds)
{
	doCollision();
}

void CollisionSystem::doCollision()
{
	updateEntityComponentPairs();

	bool doAccelerate = true;

	// Check collision between environments
	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i].first->getGameObject()->getComponent<TransformComponent>("transform")->updateRay();
		std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> collisionRes;
		if(!doAccelerate) collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(environmentComponents);
		else collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(bvh);
		//auto collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(environmentComponents);
		//auto collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(bvh);
		notifyEnvironmentCollision(i, collisionRes.first, collisionRes.second);
	}

	// Update movable game objects
	if (!doAccelerate) {
		int sum = 0;
		for (int i = 0; i < entityComponentPairs.size(); i++)
			for (int j = 0; j < i; j++) {
				sum++;
				auto collisionComponent1 = entityComponentPairs[i].first;
				auto collisionComponent2 = entityComponentPairs[j].first;
				glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
				if (glm::length(mtv) == 0) continue;  // No collision
				notifyCollision(i, j, mtv);
			}
		//std::cout << "collision: " << sum << std::endl;
	}
	else {
		hierarchicalGrid->build(entityComponentPairs);
		hierarchicalGrid->collide();
	}

	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i].first->getGameObject()->getComponent<TransformComponent>("transform")->updateRay();
	}
}

void CollisionSystem::notifyEnvironmentCollision(int index, std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (entityComponentPairs[index].second != nullptr)
		entityComponentPairs[index].second->doCollision(collisions, curPos);
}

void CollisionSystem::notifyCollision(int index1, int index2, glm::vec3 mtv)
{
	if(entityComponentPairs[index1].second != nullptr)
		entityComponentPairs[index1].second->doCollision(entityComponentPairs[index2].second, mtv);
	if(entityComponentPairs[index2].second != nullptr)
		entityComponentPairs[index2].second->doCollision(entityComponentPairs[index1].second, -mtv);
}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	auto collisionComponents = gameObject->getComponents<CollisionComponent>("collision");
	auto collisionResponseComponent = gameObject->getComponent<CollisionResponseComponent>("collisionResponse");

	for (auto collisionComponent : collisionComponents) {
		entityComponentPairs.push_back({ collisionComponent, collisionResponseComponent });
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
		if (!entityComponentPairs[i].first->getGameObject()->getActiveStatus())
			entityComponentPairs.erase(entityComponentPairs.begin() + i);
}

void CollisionSystem::buildBVH()
{
	bvh = std::make_shared<BVH>(environmentComponents);
	bvh->buildTree();
}
