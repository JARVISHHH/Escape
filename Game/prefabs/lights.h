#pragma once

#include "Game/components.h"
#include <Game/gameComponents/lightComponent.h>
#include <Engine/Game/gameSystems/tickSystem.h>
#include <Engine/Game/map/mapNode.h>

#define HEIGHT 6

std::shared_ptr<GameObject> createLights(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Map> map);
