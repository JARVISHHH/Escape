#pragma once

#include "Engine/Game/gameComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>

class CylinderComponent;

class CollisionComponent : public std::enable_shared_from_this<CollisionComponent>, public GameComponent {
public:
	CollisionComponent();

	virtual glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) = 0;
	virtual glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) = 0;
	virtual glm::mat4x4 getTransformMatrix() = 0;

	std::shared_ptr<CollisionInfo> getEnvironmentClosestCollision(glm::mat4x4 transformMatrix, std::shared_ptr<Ray> ray, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> ellipsoidTriangleCollision(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	glm::vec4 doNudge(glm::mat4x4 transformMatrix, glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
};