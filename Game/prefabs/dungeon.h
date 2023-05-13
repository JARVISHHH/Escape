#pragma once

#include <Game/prefabs/environment.h>
#include <Game/prefabs/character.h>
#include <Game/prefabs/rooms.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

#include <corecrt_math_defines.h>

#define EPSILON 0.00001
#define HEIGHT 6

//void createLight(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Map> map) {
//	if (map == nullptr) return;
//	auto& mapNodes = map->leaves;
//	std::vector<std::shared_ptr<Light>> lights;
//	lights.reserve(mapNodes.size());
//	for (auto mapNode : mapNodes) {
//		auto room = mapNode->room;
//		glm::vec3 pos = glm::vec3(room->getCenter()[0], room->getMinPoint()[1] + HEIGHT - 1, room->getCenter()[2]);
//		auto light = std::make_shared<Light>(LightType::POINT, pos, glm::vec3(5.0f));
//		lights.push_back(light);
//	}
//	Global::graphics.bindShader("phong");
//	Global::graphics.setLights(lights);
//}

void createDungeonRooms(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map, std::shared_ptr<GameObject> gameHandlerObject) {
	if (map == nullptr) return;
	bool createdTreasure = false;
	auto& mapNodes = map->leaves;
	for (auto mapNode : mapNodes) {
		// Floors && ceilings
		//createSafeRoom(gameWorld, screen, mapNode);
		//createEnemyRoom(gameWorld, screen, mapNode, gameHandlerObject);
		if (mapNode->safeRoom) {
			createSafeRoom(gameWorld, screen, mapNode);
		}
		else {
			if (!createdTreasure && mapNode->gapStarts.size() == 1 && rand() % 2) {
				createTreasureRoom(gameWorld, screen, mapNode, gameHandlerObject);
				createdTreasure = true;
			}
			else {
				auto pick = rand() % 10;
				if (pick < 5) createEnemyRoom(gameWorld, screen, mapNode, gameHandlerObject);
				else if (pick < 10) createTrapRoom(gameWorld, screen, mapNode);
			}
		}
		// Walls
		for (int i = 0; i < mapNode->gapEnds.size(); i++) {
			auto end = mapNode->gapEnds[i];
			while (findNextCorner(mapNode->gapStarts, end, mapNode->room)) {
				auto corner = nextCorner(end, mapNode->room);
				createWall(gameWorld, screen, end, corner, HEIGHT);
				end = corner;
			}
			auto start = closestStart(mapNode->gapStarts, end, mapNode->room);
			if (!checkSame(start[0], end[0]) || !checkSame(start[2], end[2])) {
				createWall(gameWorld, screen, start, end, HEIGHT);
			}
		}
	}
}

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map, std::shared_ptr<GameObject> gameHandlerObject) {
	for (auto& connector : map->getConnectors()) {
		// Walls
		glm::vec3 source = connector.first->room->getCenter(), target = connector.second->room->getCenter();
		glm::vec3 center = (source + target) / 2.0f;
		auto transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		auto angle = std::acos(glm::dot(glm::normalize(target - source), glm::vec3(1, 0, 0)));
		if (target[2] > source[2]) angle = -angle;
		transform->rotate(angle, glm::vec3(0, 1, 0));
		transform->translate(glm::vec3(center[0], (connector.first->room->getMinPoint()[1] + connector.second->room->getMinPoint()[1]) / 2.0f, center[2]));
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
		createWall(gameWorld, screen, wallPos[0].first, wallPos[1].first, HEIGHT);
		createWall(gameWorld, screen, wallPos[0].second, wallPos[1].second, HEIGHT);

		// Floor
		transform = std::make_shared<ModelTransform>();
		transform->rotate(angle, glm::vec3(0, 1, 0));
		transform->scale(glm::vec3(glm::length(wallPos[0].first - wallPos[1].first), 1.0f, 2.0f));
		auto connectorCenter = (wallPos[0].first + wallPos[1].second) / 2.0f;
		transform->translate(glm::vec3(connectorCenter[0], (connector.first->room->getMinPoint()[1] + connector.second->room->getMinPoint()[1]) / 2.0f, connectorCenter[2]));
		createEnvironment(gameWorld, screen, "plane", "connector", "connectorNormal", transform);

		// Ceiling
		transform = std::make_shared<ModelTransform>();
		transform->scale(glm::vec3(glm::length(source - target) / 2, 1.0f, 1.0f));
		transform->rotate(angle, glm::vec3(0, 1, 0));
		transform->rotate(M_PI, glm::vec3(1, 0, 0));
		transform->translate(glm::vec3(center[0], (connector.first->room->getMinPoint()[1] + connector.second->room->getMinPoint()[1]) / 2.0f, center[2]) + glm::vec3(0, HEIGHT, 0));
		createEnvironment(gameWorld, screen, "plane", "wall", "wallNormal", transform);
	}

	// create rooms
	auto characterRoom = map->mapRoot->findTopLeft();
	characterRoom->safeRoom = true;
	auto goalRoom = map->mapRoot->findTopRight();
	goalRoom->safeRoom = true;
	createDungeonRooms(gameWorld, screen, map, gameHandlerObject);

	// create character
	auto characterRoomCenter = characterRoom->room->getCenter();
	createCharacter(gameWorld, glm::vec3(characterRoomCenter[0], characterRoom->room->getMinPoint()[1] + 2, characterRoomCenter[2]) + glm::vec3(0, 1, 0), gameHandlerObject);
	// create goal
	auto goalRoomCenter = goalRoom->room->getCenter();
	createGoal(gameWorld, glm::vec3(goalRoomCenter[0], goalRoom->room->getMinPoint()[1] + 1, goalRoomCenter[2]) + glm::vec3(0, 1, 0));

	// create lighst
	//createLight(gameWorld, map);
}