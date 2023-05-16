#pragma once

#include <Game/prefabs/environment.h>
#include <Game/prefabs/spike.h>
#include <Game/prefabs/treasure.h>

#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>
#include <Game/prefabs/enemy.h>

#define EPSILON 0.00001
#define HEIGHT 6
#define HEIGHT_STAIR 0.5

bool checkSame(float a, float b);

// End to start
bool findNextCorner(glm::vec3 start, glm::vec3 end, std::shared_ptr<AABB> aabb);

bool findNextCorner(std::vector<glm::vec3>& starts, glm::vec3 end, std::shared_ptr<AABB> aabb);

glm::vec3 nextCorner(glm::vec3 end, std::shared_ptr<AABB> aabb);

glm::vec3 closestStart(std::vector<glm::vec3>& starts, glm::vec3 end, std::shared_ptr<AABB> aabb);

void createWall(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, glm::vec3 start, glm::vec3 end, float height);

void createSafeRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode);

void createMixEnemies(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode, std::shared_ptr<ModelTransform> transform, std::shared_ptr<GameObject> gameHandlerObject);

void createShootingEnemies(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode, std::shared_ptr<ModelTransform> transform, std::shared_ptr<GameObject> gameHandlerObject);

void createChasingEnemies(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode, std::shared_ptr<ModelTransform> transform, std::shared_ptr<GameObject> gameHandlerObject);

void createEnemyRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode, std::shared_ptr<GameObject> gameHandlerObject);

void createPlatform(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode);

void createTrapRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode);

void createTreasureRoom(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Screen> screen, std::shared_ptr<MapNode> mapNode, std::shared_ptr<GameObject> gameHandlerObject);