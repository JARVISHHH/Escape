#pragma once

#include <Game/components.h>
#include <Engine/Game/gameWorld.h>
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>
#include <Game/gameComponents/bulletCollisionResponse.h>
#include <Game/gameComponents/movingComponent.h>

std::shared_ptr<GameObject> createBullet(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, glm::vec3 direction, std::string bulletTag) {
	std::shared_ptr<GameObject> bulletObject = std::make_shared<GameObject>(bulletTag);

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
	std::shared_ptr<BulletCollisionResponse> collisionResponseComponent = std::make_shared<BulletCollisionResponse>(true);
	// Moving component
	std::shared_ptr<MovingComponent> movingComponent = std::make_shared<MovingComponent>(direction, 20);

	// Add components to game objects
	bulletObject->addComponent(transformComponent);
	bulletObject->addComponent(drawComponent);
	bulletObject->addComponent(collisionComponent);
	bulletObject->addComponent(collisionResponseComponent);
	bulletObject->addComponent(movingComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(bulletObject, "character");
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(movingComponent);
	gameWorld->addGameObject(bulletObject);

	bulletObject->start();

	return bulletObject;
}