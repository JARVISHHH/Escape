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
		auto targetObject = getBehaviorComponent()->getGameObject()->getGameWorld()->getGameObject(targetTag);
		if (targetObject != nullptr) targetTransform = targetObject->getComponent<TransformComponent>("transform");
	}
	//pathfindingComponent = std::static_pointer_cast<PathfindingComponent>(behaviorComponent->blackBoard["test1"]);
	pathfindingComponent = getBehaviorComponent()->getGameObject()->getComponent<PathfindingComponent>("pathfinding");
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
		if (!findTarget) {
			std::cout << "no target" << std::endl;
			status = BT_FAIL;
		}
		else {
			auto pos = targetTransform->getModelTransform()->getPos();
			status = BT_RUNNING;
			if (pathfindingComponent->getTarget()) {
				std::cout << "get target" << std::endl;
				status = BT_SUCCESS;
			}
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