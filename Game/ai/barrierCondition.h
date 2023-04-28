#pragma once

#include <Engine/Game/ai/behaviortree/condition.h>
#include <Game/components.h>
#include <Engine/Game/gameSystems/collisionSystem.h>

class BarrierCondition : public Condition {
public:
	BarrierCondition(std::string targetTag);
	void start() override;

protected:
	bool checkCondition(double seconds) override;

private:
	std::string targetTag;
	std::shared_ptr<CollisionSystem> collisionSystem = nullptr;
	std::shared_ptr<TransformComponent> transform = nullptr;
	std::shared_ptr<TransformComponent> targetTransform = nullptr;
};