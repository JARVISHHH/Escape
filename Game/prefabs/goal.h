#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createGoal(std::shared_ptr<GameWorld> gameWorld,  glm::vec3 pos)
{
	std::shared_ptr<GameObject> goalObject = std::make_shared<GameObject>("goal");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->translate(pos);
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("cylinder", "monomi");
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<CollisionResponseComponent> collisionResponse = std::make_shared<CollisionResponseComponent>(true);

	// Add components to game objects
	goalObject->addComponent(transformComponent);
	goalObject->addComponent(drawComponent);
	goalObject->addComponent(collisionComponent);
	goalObject->addComponent(collisionResponse);

	// Add the game object to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(goalObject);
	gameWorld->addGameObject(goalObject);

	return goalObject;
}