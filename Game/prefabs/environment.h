#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createEnvironment(std::shared_ptr<Screen> screen, std::string shape, std::string material, std::shared_ptr<ModelTransform> transform = nullptr)
{
	std::shared_ptr<GameObject> environmentObject = std::make_shared<GameObject>("environment");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	if (transform != nullptr) modelTransform->copy(transform);
	if (shape.compare("level") == 0) {
		modelTransform->scale(0.25);
		//modelTransform->scale(1.2);
	}
	else if (shape.compare("bvh_test") == 0) {
		modelTransform->scale(0.25);
	}

	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<EnvironmentComponent> collisionComponent = std::make_shared<EnvironmentComponent>(screen->getEnvironmentMesh(shape));

	// Add components to game objects
	environmentObject->addComponent(transformComponent);
	environmentObject->addComponent(drawComponent);
	environmentObject->addComponent(collisionComponent);

	return environmentObject;
}