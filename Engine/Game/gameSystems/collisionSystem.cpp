#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>
#include <Engine/Game/components/transformComponent.h>

CollisionSystem::CollisionSystem()
	: GameSystem("collision")
{

}

void CollisionSystem::update(double seconds)
{
	doCollision();
}

void CollisionSystem::doCollision()
{
	updateEntityComponentPairs();

	// Check collision between environments
	for (int i = 0; i < entityComponentPairs.size(); i++) {
		entityComponentPairs[i].first->getGameObject()->getComponent<TransformComponent>("transform")->updateRay();
		auto collisionRes = entityComponentPairs[i].first->ellipsoidTriangleCollision(environmentComponents);
		if (collisionRes.first.size() <= 0) continue;  // No collision
		notifyEnvironmentCollision(i, collisionRes.first, collisionRes.second);
	}

	// Update movable game objects
	for (int i = 0; i < entityComponentPairs.size(); i++)
		for (int j = 0; j < i; j++) {
			auto collisionComponent1 = entityComponentPairs[i].first;
			auto collisionComponent2 = entityComponentPairs[j].first;
			glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
			if (glm::length(mtv) == 0) continue;  // No collision
			notifyCollision(i, j, mtv);
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
	environmentComponents.push_back(environmentComponent);
}

void CollisionSystem::updateEntityComponentPairs()
{
	for (int i = entityComponentPairs.size() - 1; i >= 0; i--)
		if (!entityComponentPairs[i].first->getGameObject()->getActiveStatus())
			entityComponentPairs.erase(entityComponentPairs.begin() + i);
}
