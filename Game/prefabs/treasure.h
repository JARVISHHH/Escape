#pragma once

#include <Game/components.h>
#include <corecrt_math_defines.h>

std::shared_ptr<GameObject> createTreasure(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, glm::vec3 direction, std::string tag) {
	std::shared_ptr<GameObject> treasureObject = std::make_shared<GameObject>(tag);

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.25);
	modelTransform->rotate(std::acos(glm::dot(glm::vec3(1, 0, 0), direction)), glm::vec3(0, 1, 0));
	modelTransform->translate(pos);
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<ModelTransform> collisionTransform = std::make_shared<ModelTransform>();
	collisionTransform->scale(glm::vec3(0.2, 2, 2));
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>(collisionTransform);
	std::shared_ptr<CollisionResponseComponent> collisionResponseComponent = std::make_shared<CollisionResponseComponent>(true);

	// Add components to game objects
	treasureObject->addComponent(transformComponent);
	treasureObject->addComponent(drawComponent);
	treasureObject->addComponent(collisionComponent);
	treasureObject->addComponent(collisionResponseComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(treasureObject, "enemy");
	gameWorld->addGameObject(treasureObject);

	return treasureObject;
}

std::vector<std::string> treasureTags = {"coin", "bullet", "health"};
std::vector<std::string> treasureShapes = { "coin", "bullet", "health" };
std::vector<std::string> treasureMaterials = { "", "", "" };

std::shared_ptr<GameObject> createDoubleTreasure(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, glm::vec3 direction) {
	std::shared_ptr<GameObject> treasureObject = std::make_shared<GameObject>("treasure");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.05);
	modelTransform->translate(pos);

	// Add components to game objects
	treasureObject->addComponent(transformComponent);

	// Add children game objects
	int skip = rand() % 3;
	treasureObject->addGameObject(createTreasure(gameWorld, treasureShapes[(skip + 1) % 3], treasureMaterials[(skip + 1) % 3], pos - direction / 2.0f, direction, treasureTags[(skip + 1) % 3]));
	treasureObject->addGameObject(createTreasure(gameWorld, treasureShapes[(skip + 2) % 3], treasureMaterials[(skip + 2) % 3], pos + direction / 2.0f, direction, treasureTags[(skip + 2) % 3]));

	// Add the gameObject to systems
	gameWorld->addGameObject(treasureObject);

	return treasureObject;
}

std::shared_ptr<GameObject> createTripleTreasure(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, glm::vec3 direction) {
	std::shared_ptr<GameObject> treasureObject = std::make_shared<GameObject>("treasure");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.05);
	modelTransform->translate(pos);

	// Add components to game objects
	treasureObject->addComponent(transformComponent);

	// Add children game objects
	treasureObject->addGameObject(createTreasure(gameWorld, treasureShapes[0], treasureMaterials[0], pos - direction, direction, treasureTags[0]));
	treasureObject->addGameObject(createTreasure(gameWorld, treasureShapes[1], treasureMaterials[1], pos, direction, treasureTags[1]));
	treasureObject->addGameObject(createTreasure(gameWorld, treasureShapes[2], treasureMaterials[2], pos + direction, direction, treasureTags[2]));

	// Add the gameObject to systems
	gameWorld->addGameObject(treasureObject);

	return treasureObject;
}