#pragma once

#include <Game/components.h>
#include <Game/gameComponents/enemyMovement.h>
#include <Game/ai/scopeCondition.h>
#include <Game/ai/trackAction.h>
#include <Engine/Game/ai/behaviortree/selector.h>
#include <Engine/Game/ai/behaviortree/sequence.h>
#include <Game/ai/barrierCondition.h>
#include <Game/gameComponents/shootComponent.h>
#include <Game/ai/shootAction.h>
#include <Game/gameComponents/enemyCollisionResponse.h>
#include <Game/gameComponents/enemyHealth.h>

std::shared_ptr<GameObject> createChasingEnemy(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, std::shared_ptr<NavMesh> navMesh, std::shared_ptr<GameObject> gameHandlerObject);
std::shared_ptr<GameObject> createShootingEnemy(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, std::shared_ptr<GameObject> gameHandlerObject);