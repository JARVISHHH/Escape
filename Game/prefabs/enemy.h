#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createEnemy(std::string shape, std::string material, glm::vec3 pos)
{
	std::shared_ptr<GameObject> enemyObject = std::make_shared<GameObject>("enemy");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(pos);
	transformComponent->updateRay();
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<CollisionResponseComponent> collisionResponse = std::make_shared<CollisionResponseComponent>(true);

	// Add components to game objects
	enemyObject->addComponent(transformComponent);
	enemyObject->addComponent(drawComponent);
	enemyObject->addComponent(collisionComponent);
	enemyObject->addComponent(collisionResponse);

	return enemyObject;
}