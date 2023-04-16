#pragma once

#include <Engine/Game/gameComponent.h>
#include <Engine/Game/components/pathFindingComponent.h>

class EnemyMovement : public GameComponent {
public:
	EnemyMovement(std::string targetTag);

	void start() override;
	void update(double seconds) override;

	std::string targetTag = "";

private:
	std::shared_ptr<TransformComponent> target = nullptr;
	std::shared_ptr<PathfindingComponent> pathfinding = nullptr;
};
