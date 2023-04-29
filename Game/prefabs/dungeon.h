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
		//transform->scale(2);
		transform->scale(glm::vec3(size[0], 0.5f, size[2]));
		transform->translate(glm::vec3(center[0], center[1], center[2]));
		createEnvironment(gameWorld, screen, "box", "grass", transform);
	}
}

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map) {
	createDungeon(gameWorld, screen, map->mapRoot);
}