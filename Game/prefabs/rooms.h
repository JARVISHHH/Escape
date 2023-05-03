#pragma once

#include <Game/prefabs/environment.h>

#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

#define HEIGHT 5

void createSafeRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
	auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
	auto transform = std::make_shared<ModelTransform>();
	transform->scale(glm::vec3(size[0], 0.005f, size[2]));
	transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1], center[2]));
	createEnvironment(gameWorld, screen, "box", "ground", transform);

	transform->translate(glm::vec3(0, HEIGHT, 0));
	createEnvironment(gameWorld, screen, "box", "ground", transform);
}

void createNormalRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
	auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
	auto transform = std::make_shared<ModelTransform>();
	transform->scale(glm::vec3(size[0], 0.005f, size[2]));
	transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1], center[2]));
	createEnvironment(gameWorld, screen, "box", "ground", transform);
	mapNode->navMesh = std::make_shared<NavMesh>("./Resources/Meshes/plane.obj");
	mapNode->navMesh->bake(transform->getModelMatrix());
	createShootingEnemy(gameWorld, "cylinder", "monokuma", glm::vec3(center[0], mapNode->room->getMinPoint()[1] + 1.5, center[2]));
	createChasingEnemy(gameWorld, "cylinder", "monokuma", glm::vec3(center[0], mapNode->room->getMinPoint()[1] + 2, center[2]), mapNode->navMesh);
	
	transform->translate(glm::vec3(0, HEIGHT, 0));
	createEnvironment(gameWorld, screen, "box", "ground", transform);
}

void createPlatform(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	if (mapNode == nullptr) return;
	createPlatform(gameWorld, screen, mapNode->leftChild);
	createPlatform(gameWorld, screen, mapNode->rightChild);
	if (mapNode->room != nullptr) {
		auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
		auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(size[0], 0.005f, size[2]));
		transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1] + 0.25 + (float)rand() / RAND_MAX * (1.5 / 4), center[2]));
		createEnvironment(gameWorld, screen, "box", "ground", transform);
	}
}

void createTrapRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	std::shared_ptr<Map> roomMap = std::make_shared<Map>(mapNode->room, glm::vec3(2, 0.5, 1.5), 0.5, 10);
	roomMap->generateMap();
	createPlatform(gameWorld, screen, roomMap->mapRoot);

	// Ceiling
	auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
	auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
	auto transform = std::make_shared<ModelTransform>();
	transform->scale(glm::vec3(size[0], 0.005f, size[2]));
	transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1], center[2]));
	transform->translate(glm::vec3(0, HEIGHT, 0));
	createEnvironment(gameWorld, screen, "box", "ground", transform);
}