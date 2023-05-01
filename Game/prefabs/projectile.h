#pragma once

#include <Game/components.h>
#include <Game/gameComponents/projectileCollisionResponse.h>
#include <Game/gameComponents/movingComponent.h>
#include <Engine/Game/gameWorld.h>
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>

std::shared_ptr<GameObject> createProjectile(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, glm::vec3 direction) {
	std::shared_ptr<GameObject> enemyObject = std::make_shared<GameObject>("enemy");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.05);
	modelTransform->translate(pos);
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<ProjectileCollisionResponse> collisionResponseComponent = std::make_shared<ProjectileCollisionResponse>(true);
	// Moving component
	std::shared_ptr<MovingComponent> movingComponent = std::make_shared<MovingComponent>(direction, 5);

	// Add components to game objects
	enemyObject->addComponent(transformComponent);
	enemyObject->addComponent(drawComponent);
	enemyObject->addComponent(collisionComponent);
	enemyObject->addComponent(collisionResponseComponent);
	enemyObject->addComponent(movingComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(enemyObject, "projectile");
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(movingComponent);
	gameWorld->addGameObject(enemyObject);

	enemyObject->start();

	return enemyObject;
}