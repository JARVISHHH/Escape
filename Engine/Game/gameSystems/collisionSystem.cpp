#include "collisionSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"

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
	for (int i = 0; i < gameObjects.size(); i++)
		for (int j = 0; j < i; j++) {
			auto collisionComponent1 = gameObjects[i]->getComponent<CollisionComponent>("collision");
			auto collisionComponent2 = gameObjects[j]->getComponent<CollisionComponent>("collision");
			if (collisionComponent1->checkCollision(collisionComponent2)) std::cout << "yes" << std::endl;
		}
}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
}