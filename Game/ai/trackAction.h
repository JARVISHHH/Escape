#pragma once

#include <Engine/Game/ai/behaviortree/action.h>
#include <Game/components.h>

class TrackAction : public Action {
public:
	TrackAction(glm::vec3 targetPosition);
	TrackAction(std::string targetTag);
	void start() override;
	void reset() override;

protected:
	BTStatus doAction(double seconds);

private:
	bool trackObject;
	glm::vec3 targetPosition;
	std::string targetTag;
	std::shared_ptr<TransformComponent> targetTransform = nullptr;
	std::shared_ptr<PathfindingComponent> pathfindingComponent = nullptr;

};