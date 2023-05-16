#pragma once

#include <Game/prefabs/environment.h>
#include <Game/prefabs/character.h>
#include <Game/prefabs/rooms.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

#include <corecrt_math_defines.h>
#include <Game/prefabs/goal.h>

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

void createDungeonRooms(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map, std::shared_ptr<GameObject> gameHandlerObject);

void createDungeon(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<Map> map, std::shared_ptr<GameObject> gameHandlerObject);