#pragma once

#include <Game/components.h>
#include <Game/gameComponents/gameHandlerComponent.h>

std::shared_ptr<GameObject> createGameHandler(std::shared_ptr<GameWorld> gameWorld, float maxTime = 60);
