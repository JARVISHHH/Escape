#pragma once

#include <Game/components.h>
#include <corecrt_math_defines.h>

std::shared_ptr<GameObject> createTreasure(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, glm::vec3 direction, std::string tag);
std::shared_ptr<GameObject> createDoubleTreasure(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, glm::vec3 direction);

std::shared_ptr<GameObject> createTripleTreasure(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, glm::vec3 direction);