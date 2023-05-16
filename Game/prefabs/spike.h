#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createSpike(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::string shape, std::string material, std::shared_ptr<ModelTransform> modelTransform);