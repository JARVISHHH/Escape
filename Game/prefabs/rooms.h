#pragma once

#include <Game/prefabs/environment.h>
#include <Game/prefabs/spike.h>

#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

#define EPSILON 0.00001
#define HEIGHT 5
#define HEIGHT_STAIR 0.5

bool checkSame(float a, float b) {
	if (std::abs(a - b) < EPSILON) return true;
	return false;
}

// End to start
bool findNextCorner(glm::vec3 start, glm::vec3 end, std::shared_ptr<AABB> aabb) {
	if (!checkSame(start[0], end[0]) && !checkSame(start[2], end[2])) return true;
	if (checkSame(start[0], end[0]) && checkSame(start[2], end[2])) return false;
	auto maxPoint = aabb->getMaxPoint(), minPoint = aabb->getMinPoint();
	if (checkSame(start[0], end[0])) {
		if (checkSame(end[0], maxPoint[0])) return end[2] < start[2];
		if (checkSame(end[0], minPoint[0])) return end[2] > start[2];
	}
	if (checkSame(end[2], start[2])) {
		if (checkSame(end[2], maxPoint[2])) return end[0] > start[0];
		if (checkSame(end[2], minPoint[2])) return end[0] < start[0];
	}
	return true;
}

bool findNextCorner(std::vector<glm::vec3>& starts, glm::vec3 end, std::shared_ptr<AABB> aabb) {
	for (auto start : starts)
		if (!findNextCorner(start, end, aabb))
			return false;
	return true;
}

glm::vec3 nextCorner(glm::vec3 end, std::shared_ptr<AABB> aabb) {
	glm::vec3 corner;
	auto maxPoint = aabb->getMaxPoint(), minPoint = aabb->getMinPoint();
	if (checkSame(end[0], maxPoint[0]) && !checkSame(end[2], minPoint[2])) corner = glm::vec3(end[0], end[1], minPoint[2]);
	else if (checkSame(end[0], minPoint[0]) && !checkSame(end[2], maxPoint[2])) corner = glm::vec3(end[0], end[1], maxPoint[2]);
	else if (checkSame(end[2], maxPoint[2]) && !checkSame(end[0], maxPoint[0])) corner = glm::vec3(maxPoint[0], end[1], end[2]);
	else corner = glm::vec3(minPoint[0], end[1], end[2]);
	return corner;
}

glm::vec3 closestStart(std::vector<glm::vec3>& starts, glm::vec3 end, std::shared_ptr<AABB> aabb) {
	std::vector<glm::vec3> candidates;
	for (auto start : starts)
		if (!findNextCorner(start, end, aabb))
			candidates.push_back(start);
	glm::vec3 res = nextCorner(end, aabb);
	auto maxPoint = aabb->getMaxPoint(), minPoint = aabb->getMinPoint();
	if (checkSame(end[0], maxPoint[0]) && !checkSame(end[2], minPoint[2])) {
		for (auto start : candidates)
			res[2] = std::max(res[2], start[2]);
	}
	else if (checkSame(end[0], minPoint[0]) && !checkSame(end[2], maxPoint[2])) {
		for (auto start : candidates)
			res[2] = std::min(res[2], start[2]);
	}
	else if (checkSame(end[2], maxPoint[2]) && !checkSame(end[0], maxPoint[0])) {
		for (auto start : candidates)
			res[0] = std::min(res[0], start[0]);
	}
	else {
		for (auto start : candidates)
			res[0] = std::max(res[0], start[0]);
	}
	return res;
}

void createWall(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, glm::vec3 start, glm::vec3 end, float height) {
	auto transform = std::make_shared<ModelTransform>();
	transform->scale(glm::vec3(glm::length(end - start) / 2, height, 0.5f));
	auto angle = std::acos(glm::dot(glm::normalize(end - start), glm::vec3(1, 0, 0)));
	if (end[2] > start[2]) angle = -angle;
	transform->rotate(angle, glm::vec3(0, 1, 0));
	transform->translate((start + end) / 2.0f + glm::vec3(0, height / 2, 0));
	createEnvironment(gameWorld, screen, "wall", "wall", transform);
}

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
		transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1] + 1.2 + (float)rand() / RAND_MAX * 1.3, center[2]));
		createEnvironment(gameWorld, screen, "box", "ground", transform);
	}
}

void createTrapRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	// Platforms
	std::shared_ptr<Map> roomMap = std::make_shared<Map>(mapNode->room, glm::vec3(2, 0.5, 1.5), 0.5, 10);
	roomMap->generateMap();
	createPlatform(gameWorld, screen, roomMap->mapRoot);

	// Spikes
	auto spikeTransform = std::make_shared<ModelTransform>();
	auto maxPoint = mapNode->room->getMaxPoint(), minPoint = mapNode->room->getMinPoint();
	int xNum = maxPoint[0] - minPoint[0], zNum = maxPoint[2] - minPoint[2];
	float xSize = (maxPoint[0] - minPoint[0]) / xNum, zSize = (maxPoint[2] - minPoint[2]) / zNum;
	spikeTransform->scale(glm::vec3(xSize, 1, zSize));
	spikeTransform->translate(glm::vec3(minPoint[0] + xSize / 2.0f, minPoint[1] + 0.05, minPoint[2] + zSize / 2.0f));
	for (float z = minPoint[2] + zSize / 2.0f; z < maxPoint[2]; z += zSize) {
		int n = 0;
		for (float x = minPoint[0] + xSize / 2.0f; x < maxPoint[0]; x += xSize) {
			createSpike(gameWorld, screen, "spike", "", spikeTransform);
			spikeTransform->translate(glm::vec3(xSize, 0, 0));
			n++;
		}
		spikeTransform->translate(glm::vec3(-n * xSize, 0, zSize));
	}

	// Stair
	for (int i = 0; i < mapNode->gapStarts.size(); i++) {
		auto start = mapNode->gapStarts[i], end = mapNode->gapEnds[i];
		while (findNextCorner(end, start, mapNode->room)) {
			auto corner = nextCorner(start, mapNode->room);
			createWall(gameWorld, screen, start, corner, HEIGHT_STAIR);
			start = corner;
		}
		if (!checkSame(start[0], end[0]) || !checkSame(start[2], end[2])) {
			createWall(gameWorld, screen, start, end, HEIGHT_STAIR);
		}
	}

	// Ceiling
	auto center = (mapNode->room->getMaxPoint() + mapNode->room->getMinPoint()) / 2.0f;
	auto size = (mapNode->room->getMaxPoint() - mapNode->room->getMinPoint()) / 2.0f;
	auto transform = std::make_shared<ModelTransform>();
	transform->scale(glm::vec3(size[0], 0.005f, size[2]));
	transform->translate(glm::vec3(center[0], mapNode->room->getMinPoint()[1], center[2]));
	transform->translate(glm::vec3(0, HEIGHT, 0));
	createEnvironment(gameWorld, screen, "box", "ground", transform);
}