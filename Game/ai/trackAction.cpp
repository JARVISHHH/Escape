#include "trackAction.h"

TrackAction::TrackAction(glm::vec3 targetPosition)
	:targetPosition(targetPosition), trackObject(false)
{
}

TrackAction::TrackAction(std::string targetTag)
	: targetTag(targetTag), trackObject(true)
{
}

void TrackAction::start()
{
	if (trackObject) {
		auto targetObjects = behaviorComponent->getGameObject()->getGameWorld()->getGameObjects(targetTag);
		if (targetObjects.size() > 0) targetTransform = targetObjects[0]->getComponent<TransformComponent>("transform");
	}
	pathfindingComponent = std::static_pointer_cast<PathfindingComponent>(behaviorComponent->blackBoard["test1"]);
	//pathfindingComponent = behaviorComponent->getGameObject()->getComponent<PathfindingComponent>("pathfinding");
	std::cout << behaviorComponent->blackBoard["test1"] << std::endl;
}

void TrackAction::reset()
{
	if (status == BT_RUNNING) pathfindingComponent->isStop = true;
	status = BT_FAIL;
}

BTStatus TrackAction::doAction(double seconds)
{
	if (trackObject) {
		bool findTarget = pathfindingComponent->setDestination(targetTransform->getModelTransform()->getPos());
		if (!findTarget) status = BT_FAIL;
		else {
			status = BT_RUNNING;
			if (pathfindingComponent->getTarget()) status = BT_SUCCESS;
		}
	}
	else {
		if (status == BT_RUNNING) {
			if (pathfindingComponent->getTarget()) status = BT_SUCCESS;
		}
		else {
			bool findTarget = pathfindingComponent->setDestination(targetPosition);
			if (!findTarget) status = BT_FAIL;
			else {
				status = BT_RUNNING;
			}
		}
	}
	pathfindingComponent->isStop = false;

	return status;
}