#pragma once

#include "Game/components.h"
#include <Game/gameComponents/characterHealth.h>
#include <Game/gameComponents/characterCollisionResponse.h>
#include <Engine/Game/gameSystems/cameraSystem.h>
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/physicsSystem.h>
#include <Engine/Game/gameSystems/characterControllerSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>
#include <Game/gameComponents/characterShoot.h>

#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

std::shared_ptr<GameObject> createCharacter(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, std::shared_ptr<GameObject> gameHandlerObject);

std::shared_ptr<GameObject> createMovingCharacter(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, std::shared_ptr<Map> map, std::shared_ptr<MapNode> mapNode);