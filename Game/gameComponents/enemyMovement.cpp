#include "enemyMovement.h"

EnemyMovement::EnemyMovement(std::string targetTag)
	: GameComponent("enemyMovement"), targetTag(targetTag)
{

}

void EnemyMovement::start()
{
	auto targetObjects = gameObject->getGameWorld()->getGameObjects(targetTag);
	if (targetObjects.size() != 0) target = targetObjects[0]->getComponent<TransformComponent>("transform");
	pathfinding = gameObject->getComponent<PathfindingComponent>("pathfinding");
}

void EnemyMovement::update(double seconds)
{
	if (target == nullptr || pathfinding == nullptr) return;
	pathfinding->setDestination(target->getModelTransform()->getPos());
}