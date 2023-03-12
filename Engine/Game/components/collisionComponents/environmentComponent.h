#pragma once

#include <vector>
#include <memory>
#include <Engine/Mesh/triangle.h>

#include "Engine/Game/gameComponent.h"

class EnvironmentComponent : public GameComponent {
public:
	EnvironmentComponent(std::vector<std::shared_ptr<Triangle>> triangles);
	std::shared_ptr<CollisionInfo> getClosestCollision(glm::mat4x4 transformMatrix, std::shared_ptr<Ray> ray);

private:
	std::vector<std::shared_ptr<Triangle>> triangles;
};