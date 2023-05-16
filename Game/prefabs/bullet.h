#pragma once

#include <Game/components.h>
#include <Engine/Game/gameWorld.h>
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>
#include <Game/gameComponents/bulletCollisionResponse.h>
#include <Game/gameComponents/movingComponent.h>

std::shared_ptr<GameObject> createBullet(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, glm::vec3 direction, std::string bulletTag);