#include "physicsSystem.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/physicsComponent.h"
#include <Engine/Game/components/transformComponent.h>

PhysicsSystem::PhysicsSystem() : GameSystem("physics")
{
}

void PhysicsSystem::update(double seconds)
{
	updateGameObjects();
	for (auto gameObject : gameObjects) {
		auto physicsComponent = gameObject->getComponent<PhysicsComponent>("physics");

		if (physicsComponent == nullptr) continue;

		physicsComponent->update(seconds);
	}
}

void PhysicsSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
}

void PhysicsSystem::updateGameObjects()
{
	for (int i = gameObjects.size() - 1; i >= 0; i--)
		if (!gameObjects[i]->getActiveStatus())
			gameObjects.erase(gameObjects.begin() + i);
}