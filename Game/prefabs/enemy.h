#pragma once

#include <Game/components.h>
#include <Game/gameComponents/enemyMovement.h>

std::shared_ptr<GameObject> createEnemy(std::string shape, std::string material, glm::vec3 pos, std::shared_ptr<NavMesh> navMesh)
{
	std::shared_ptr<GameObject> enemyObject = std::make_shared<GameObject>("enemy");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.5);
	modelTransform->translate(pos);
	transformComponent->updateRay();
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<CollisionResponseComponent> collisionResponse = std::make_shared<CollisionResponseComponent>(true);
	// AI component
	std::shared_ptr<PathfindingComponent> pathfindingComponent = std::make_shared<PathfindingComponent>(navMesh);
	// Moving component
	std::shared_ptr<EnemyMovement> enemyMovement = std::make_shared<EnemyMovement>("character");

	// Add components to game objects
	enemyObject->addComponent(transformComponent);
	enemyObject->addComponent(drawComponent);
	enemyObject->addComponent(collisionComponent);
	enemyObject->addComponent(collisionResponse);
	enemyObject->addComponent(pathfindingComponent);
	enemyObject->addComponent(enemyMovement);

	return enemyObject;
}