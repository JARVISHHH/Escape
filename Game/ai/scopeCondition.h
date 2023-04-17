#pragma once

#include <Engine/Game/ai/behaviortree/condition.h>
#include <Game/components.h>

class ScopeCondition : public Condition {
public:
	ScopeCondition(std::string targetTag, float maxDistance, std::shared_ptr<NavMesh> navMesh);
	void start() override;

protected:
	bool checkCondition(double seconds) override;

private:
	std::string targetTag;
	std::shared_ptr<TransformComponent> transform = nullptr;
	std::shared_ptr<TransformComponent> targetTransform = nullptr;

	float maxDistance;
	std::shared_ptr<NavMesh> navMesh;
};