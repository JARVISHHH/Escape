#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"

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
	for (int i = 0; i < componentPairs.size(); i++)
		for (int j = 0; j < i; j++) {
			auto collisionComponent1 = componentPairs[i].first;
			auto collisionComponent2 = componentPairs[j].first;
			glm::vec3 mtv = collisionComponent1->checkCollision(collisionComponent2);
			if (glm::length(mtv) == 0) continue;  // No collision
			notifyCollision(i, j, mtv);
		}
}

void CollisionSystem::notifyCollision(int index1, int index2, glm::vec3 mtv)
{
	if(componentPairs[index1].second != nullptr)
		componentPairs[index1].second->doCollision(componentPairs[index2].second, mtv);
	if(componentPairs[index2].second != nullptr)
		componentPairs[index2].second->doCollision(componentPairs[index1].second, -mtv);
}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	auto collisionComponents = gameObject->getComponents<CollisionComponent>("collision");
	auto collisionResponseComponent = gameObject->getComponent<CollisionResponseComponent>("collisionResponse");

	for (auto collisionComponent : collisionComponents) {
		componentPairs.push_back({ collisionComponent, collisionResponseComponent });
	}
}
