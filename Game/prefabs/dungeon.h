#pragma once

#include <Game/prefabs/environment.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

void createWall(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, glm::vec3 start, glm::vec3 end, float height) {
	auto transform = std::make_shared<ModelTransform>();
	//transform->scale(1 / 20.0);
	transform->scale(glm::vec3(glm::length(end - start), height, 1.0f));
	transform->rotate(std::acos(glm::dot(glm::normalize(end - start), glm::vec3(1, 0, 0))), glm::vec3(0, 1, 0));
	transform->translate((start + end) / 2.0f + glm::vec3(0, height / 2, 0));
	createEnvironment(gameWorld, screen, "cube", "wall", transform);
}

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
		// Floor
		glm::vec3 source = connector.first->room->getCenter(), target = connector.second->room->getCenter();
		glm::vec3 center = (source + target) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		transform->rotate(std::acos(glm::dot(glm::normalize(target - source), glm::vec3(1, 0, 0))), glm::vec3(0, 1, 0));
		transform->translate(center);
		createEnvironment(gameWorld, screen, "plane", "ground", transform);

		// Ceiling

		// Walls
		// Get intersection
		std::vector<std::pair<glm::vec3, glm::vec3>> wallPos;
		glm::vec4 direction1(1, 0, 0, 0);
		direction1 = transform->getModelMatrix() * direction1;
		for (auto node : { connector.first, connector.second }) {
			glm::vec4 direction2(0, 0, -1, 0);
			direction2 = transform->getModelMatrix() * direction2;
			auto intersection1 = node->intersect(node->room->getCenter() + direction2, direction1);
			auto intersection2 = node->intersect(node->room->getCenter() - direction2, direction1);
			node->gaps.push_back({intersection1, intersection2});
			wallPos.push_back({ intersection1, intersection2 });
			direction1 = -direction1;
		}
		// Build walls
		createWall(gameWorld, screen, wallPos[0].first, wallPos[1].first, 5);
		createWall(gameWorld, screen, wallPos[0].second, wallPos[1].second, 5);
	}
	// Area walls
	createDungeon(gameWorld, screen, map->mapRoot);
}