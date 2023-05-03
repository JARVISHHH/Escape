#pragma once

#include <Game/prefabs/environment.h>
#include <Game/prefabs/character.h>
#include <Game/prefabs/rooms.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

#include <corecrt_math_defines.h>

#define EPSILON 0.00001

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

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode) {
	if (mapNode == nullptr) return;
	createDungeon(gameWorld, screen, mapNode->leftChild);
	createDungeon(gameWorld, screen, mapNode->rightChild);
	if (mapNode->room != nullptr) {
		// Floors && ceilings
		if (mapNode->safeRoom) {
			createSafeRoom(gameWorld, screen, mapNode);
		}
		else {
			//createNormalRoom(gameWorld, screen, mapNode);
			createTrapRoom(gameWorld, screen, mapNode);
		}
		// Walls
		for (int i = 0; i < mapNode->gapEnds.size(); i++) {
			auto end = mapNode->gapEnds[i];
			while (findNextCorner(mapNode->gapStarts, end, mapNode->room)) {
				auto corner = nextCorner(end, mapNode->room);
				createWall(gameWorld, screen, end, corner, 2);
				end = corner;
			}
			auto start = closestStart(mapNode->gapStarts, end, mapNode->room);
			if (!checkSame(start[0], end[0]) || !checkSame(start[2], end[2])) {
				createWall(gameWorld, screen, start, end, 2);
			}
		}
	}
}

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map, std::shared_ptr<GameObject> gameHandlerObject) {
	for (auto& connector : map->getConnectors()) {
		// Floor
		glm::vec3 source = connector.first->room->getCenter(), target = connector.second->room->getCenter();
		glm::vec3 center = (source + target) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		auto angle = std::acos(glm::dot(glm::normalize(target - source), glm::vec3(1, 0, 0)));
		if (target[2] > source[2]) angle = -angle;
		transform->rotate(angle, glm::vec3(0, 1, 0));
		transform->translate(glm::vec3(center[0], (connector.first->room->getMinPoint()[1] + connector.second->room->getMinPoint()[1]) / 2.0f, center[2]));
		createEnvironment(gameWorld, screen, "plane", "ground", transform);

		// Walls
		// Get intersection
		std::vector<std::pair<glm::vec3, glm::vec3>> wallPos;
		glm::vec4 direction1(1, 0, 0, 0);
		direction1 = glm::normalize(transform->getModelMatrix() * direction1);
		glm::vec4 direction2(0, 0, 1, 0);
		direction2 = transform->getModelMatrix() * direction2;
		{
			auto node = connector.first;
			auto center = node->room->getCenter();
			auto origin = glm::vec4(center[0], node->room->getMinPoint()[1], center[2], 1);
			auto intersection1 = node->intersect(origin + direction2, direction1);
			auto intersection2 = node->intersect(origin - direction2, direction1);
			node->gapStarts.push_back(intersection1);
			node->gapEnds.push_back(intersection2);
			wallPos.push_back({ intersection1 - glm::vec3(direction1) * 0.25f, intersection2 - glm::vec3(direction1) * 0.25f });
		}
		{
			auto node = connector.second;
			direction1 = -direction1;
			auto center = node->room->getCenter();
			auto origin = glm::vec4(center[0], node->room->getMinPoint()[1], center[2], 1);
			auto intersection1 = node->intersect(origin + direction2, direction1);
			auto intersection2 = node->intersect(origin - direction2, direction1);
			node->gapStarts.push_back(intersection2);
			node->gapEnds.push_back(intersection1);
			wallPos.push_back({ intersection1 - glm::vec3(direction1) * 0.25f, intersection2 - glm::vec3(direction1) * 0.25f });
		}
		// Build walls
		createWall(gameWorld, screen, wallPos[0].first, wallPos[1].first, 2);
		createWall(gameWorld, screen, wallPos[0].second, wallPos[1].second, 2);

		// Ceiling
		transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		transform->rotate(angle, glm::vec3(0, 1, 0));
		transform->rotate(M_PI, glm::vec3(1, 0, 0));
		transform->translate(glm::vec3(center[0], (connector.first->room->getMinPoint()[1] + connector.second->room->getMinPoint()[1]) / 2.0f, center[2]) + glm::vec3(0, 3, 0));
		createEnvironment(gameWorld, screen, "plane", "ground", transform);
	}

	// create character
	auto characterRoom = map->mapRoot->findTopLeft();
	characterRoom->safeRoom = true;
	auto characterRoomCenter = characterRoom->room->getCenter();
	createCharacter(gameWorld, glm::vec3(characterRoomCenter[0], characterRoom->room->getMinPoint()[1] + 1, characterRoomCenter[2]) + glm::vec3(0, 1, 0), gameHandlerObject);

	// create goal
	auto goalRoom = map->mapRoot->findTopRight();
	goalRoom->safeRoom = true;
	auto goalRoomCenter = goalRoom->room->getCenter();
	createGoal(gameWorld, glm::vec3(goalRoomCenter[0], goalRoom->room->getMinPoint()[1] + 1, goalRoomCenter[2]) + glm::vec3(0, 1, 0));

	// create rooms
	createDungeon(gameWorld, screen, map->mapRoot);
}