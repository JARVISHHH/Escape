#pragma once

#include <Game/components.h>
#include <Game/gameComponents/enemyMovement.h>
#include <Game/ai/scopeCondition.h>
#include <Game/ai/trackAction.h>
#include <Engine/Game/ai/behaviortree/selector.h>
#include <Engine/Game/ai/behaviortree/sequence.h>

std::shared_ptr<GameObject> createEnemy(std::shared_ptr<GameWorld> gameWorld, std::string shape, std::string material, glm::vec3 pos, std::shared_ptr<NavMesh> navMesh)
{
	std::shared_ptr<GameObject> enemyObject = std::make_shared<GameObject>("enemy");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.5);
	modelTransform->translate(pos);
	transformComponent->updateRay();
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>(shape, material);
	// Collision component
	std::shared_ptr<CylinderComponent> collisionComponent = std::make_shared<CylinderComponent>();
	std::shared_ptr<CollisionResponseComponent> collisionResponse = std::make_shared<CollisionResponseComponent>(true);
	// AI component
	std::shared_ptr<PathfindingComponent> pathfindingComponent = std::make_shared<PathfindingComponent>(navMesh);
	std::shared_ptr<BehaviorComponent> behaviorComponent = std::make_shared<BehaviorComponent>();
	behaviorComponent->blackBoard["test1"] = pathfindingComponent;
	behaviorComponent->blackBoard["test2"] = behaviorComponent;
	std::cout << "original: " << pathfindingComponent << " " << behaviorComponent << std::endl;
	std::shared_ptr<Selector> root = std::make_shared<Selector>();
	//std::shared_ptr<Sequence> backBaseSequence = std::make_shared<Sequence>();
	//std::shared_ptr<ScopeCondition> scopeCondition = std::make_shared<ScopeCondition>("character", 30, navMesh);
	//std::shared_ptr<TrackAction> trackBase = std::make_shared<TrackAction>(pos);
	std::shared_ptr<TrackAction> trackTarget = std::make_shared<TrackAction>("character");
	//backBaseSequence->addChild(scopeCondition);
	//backBaseSequence->addChild(trackBase);
	//root->addChild(backBaseSequence);
	root->addChild(trackTarget);
	behaviorComponent->setRoot(root);
	// Moving component
	//std::shared_ptr<EnemyMovement> enemyMovement = std::make_shared<EnemyMovement>("character");

	// Add components to game objects
	enemyObject->addComponent(transformComponent);
	enemyObject->addComponent(drawComponent);
	enemyObject->addComponent(collisionComponent);
	enemyObject->addComponent(collisionResponse);
	enemyObject->addComponent(pathfindingComponent);
	enemyObject->addComponent(behaviorComponent);
	//enemyObject->addComponent(enemyMovement);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(pathfindingComponent);
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(behaviorComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(enemyObject);
	gameWorld->addGameObject(enemyObject);

	return enemyObject;
}