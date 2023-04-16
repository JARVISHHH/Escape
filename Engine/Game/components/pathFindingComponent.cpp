#include "pathFindingComponent.h"

PathfindingComponent::PathfindingComponent()
	: GameComponent("pathfinding"), currentIndex(0)
{
	
}

PathfindingComponent::PathfindingComponent(std::shared_ptr<NavMesh> navMesh)
	: GameComponent("pathfinding"), navMesh(navMesh), currentIndex(0)
{

}

void PathfindingComponent::start()
{
	transformComponent = gameObject->getComponent<TransformComponent>("transform");
}

void PathfindingComponent::update(double seconds)
{
	if (currentIndex >= path.size()) return;
	glm::vec3 currentPos = transformComponent->getModelTransform()->getPos();
	double leftTime = seconds;
	while (leftTime > 0 && currentIndex < path.size()) {
		auto nextPos = path[currentIndex];
		double takeTime = glm::length(nextPos - currentPos) / speed;
		if (takeTime >= leftTime) {
			auto direction = glm::normalize(nextPos - currentPos);
			currentPos = currentPos + direction * speed * (float)leftTime;
			leftTime = -1;
		}
		else {
			leftTime -= takeTime;
			currentPos = nextPos;
			currentIndex++;
		}
	}
	transformComponent->getModelTransform()->setPos(currentPos);
	//std::cout << "current pos: " << currentPos[0] << " " << currentPos[1] << " " << currentPos[2] << std::endl;
}

void PathfindingComponent::setDestination(glm::vec3 targetPosition)
{
	if (navMesh == nullptr) return;
	//std::cout << "targetPosition: " << targetPosition[0] << " " << targetPosition[1] << " " << targetPosition[2] << std::endl;
	if (glm::length(targetPosition - targetPos) < 0.00001) return;
	targetPos = targetPosition;
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	path = navMesh->pathFinding(transformComponent->getModelTransform()->getPos(), targetPosition);
	currentIndex = 0;
}