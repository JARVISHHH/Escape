#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createEnvironment(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::string shape, std::string material = "", std::string materialNormal = "", std::shared_ptr<ModelTransform> transform = nullptr)
{
	std::shared_ptr<GameObject> environmentObject = std::make_shared<GameObject>("environment");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	if (transform != nullptr) modelTransform->copy(transform);

	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material, materialNormal);
	// Collision component
	std::shared_ptr<EnvironmentComponent> collisionComponent = std::make_shared<EnvironmentComponent>(screen->getEnvironmentMesh(shape));

	// Add components to game objects
	environmentObject->addComponent(transformComponent);
	environmentObject->addComponent(drawComponent);
	environmentObject->addComponent(collisionComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addEnvironmentObject(environmentObject);
	gameWorld->addGameObject(environmentObject);

	return environmentObject;
}