#pragma once

#include <Engine/Game/gameComponent.h>
#include <Engine/Game/ai/navmesh/navmesh.h>
#include <Engine/Game/components/transformComponent.h>

class PathfindingComponent: public GameComponent {
public:
	PathfindingComponent();
	PathfindingComponent(std::shared_ptr<NavMesh> navMesh);

	void start() override;
	void update(double seconds) override;

	void setDestination(glm::vec3 targetPosition);

	void setNavMesh(std::shared_ptr<NavMesh> navMesh) { this->navMesh = navMesh; }

	float speed = 2;

private:
	std::shared_ptr<TransformComponent> transformComponent = nullptr;
	std::shared_ptr<NavMesh> navMesh = nullptr;
	glm::vec3 targetPos = { 100000, 100000, 100000 };
	std::vector<glm::vec3> path;
	int currentIndex;
};