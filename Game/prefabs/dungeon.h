#pragma once

#include <Game/prefabs/environment.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	if (mapNode == nullptr) return;
	createDungeon(gameWorld, screen, mapNode->leftChild);
	createDungeon(gameWorld, screen, mapNode->rightChild);
	if (mapNode->room != nullptr) {
		auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
		auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		//transform->scale(1 / 20.0);
		transform->scale(glm::vec3(size[0], 1.0f, size[2]));
		transform->translate(glm::vec3(center[0], center[1], center[2]));
		createEnvironment(gameWorld, screen, "plane", "ground", transform);
		mapNode->navMesh = std::make_shared<NavMesh>("./Resources/Meshes/plane.obj");
		mapNode->navMesh->bake(transform->getModelMatrix());
		createChasingEnemy(gameWorld, "cylinder", "monokuma", glm::vec3(center[0], center[1] + 2, center[2]), mapNode->navMesh);
	}
}

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map) {
	for (auto& connector : map->getConnectors()) {
		glm::vec3 source = connector.first, target = connector.second;
		std::cout << "source: " << source[1] << " target: " << target[1] << std::endl;
		glm::vec3 center = (source + target) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		transform->rotate(std::acos(glm::dot(glm::normalize(target - source), glm::vec3(1, 0, 0))), glm::vec3(0, 1, 0));
		std::cout << "center: " << center[1] << std::endl;
		transform->translate(center);
		createEnvironment(gameWorld, screen, "plane", "ground", transform);
	}
	createDungeon(gameWorld, screen, map->mapRoot);
}