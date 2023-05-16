#include "spike.h"
#include <Engine/Game/gameSystems/drawSystem.h>

std::shared_ptr<GameObject> createSpike(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::string shape, std::string material, std::shared_ptr<ModelTransform> modelTransform) {
	std::shared_ptr<GameObject> spikeObject = std::make_shared<GameObject>("spike");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	transformComponent->getModelTransform()->copy(modelTransform);

	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<EnvironmentComponent> collisionComponent = std::make_shared<EnvironmentComponent>(screen->getEnvironmentMesh(shape));

	// Add components to game objects
	spikeObject->addComponent(transformComponent);
	spikeObject->addComponent(drawComponent);
	spikeObject->addComponent(collisionComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addEnvironmentObject(spikeObject);
	gameWorld->addGameObject(spikeObject);

	return spikeObject;
}