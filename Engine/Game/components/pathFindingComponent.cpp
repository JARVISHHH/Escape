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
	transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
}

void PathfindingComponent::update(double seconds)
{
	if (currentIndex >= path.size() || isStop) return;
	glm::vec3 currentPos = transformComponent->getModelTransform()->getPos();
	double leftTime = seconds;
	while (leftTime > 0 && currentIndex < path.size()) {
		auto nextPos = path[currentIndex] + glm::vec3(0, transformComponent->getModelTransform()->getScale()[1] * 0.5f, 0);
		double takeTime = glm::length(nextPos - currentPos) / speed;
		if (takeTime >= leftTime) {
			auto direction = glm::normalize(nextPos - currentPos);
			currentPos = currentPos + direction * speed * (float)leftTime;
			leftTime = -1;
			//std::cout << "next pos: " << nextPos[0] << " " << nextPos[1] << " " << nextPos[2] << std::endl;
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

bool PathfindingComponent::setDestination(glm::vec3 targetPosition)
{
	if (navMesh == nullptr) return false;
	//std::cout << "targetPosition: " << targetPosition[0] << " " << targetPosition[1] << " " << targetPosition[2] << std::endl;
	if (glm::length(targetPosition - targetPos) < 0.01) return true;
	targetPos = targetPosition;
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	path = navMesh->pathFinding(transformComponent->getModelTransform()->getPos(), targetPosition);
	currentIndex = 0;
	//std::cout << "path size: " << path.size() << std::endl;
	return path.size() > 0;
}

bool PathfindingComponent::getTarget()
{
	if (currentIndex >= path.size()) return true;
	return false;
}