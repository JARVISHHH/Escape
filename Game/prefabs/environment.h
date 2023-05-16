#pragma once

#include <Game/components.h>

std::shared_ptr<GameObject> createEnvironment(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::string shape, std::string material = "", std::string materialNormal = "", std::shared_ptr<ModelTransform> transform = nullptr);