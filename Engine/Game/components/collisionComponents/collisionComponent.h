#pragma once

#include "Engine/Game/gameComponent.h"
#include <Engine/Game/components/collisionComponents/environmentComponent.h>
#include <Engine/Game/collision/bvh.h>
#include <Engine/Game/gameSystems/collisionSystem.h>

class CylinderComponent;
class GridNode;

class CollisionComponent : public std::enable_shared_from_this<CollisionComponent>, public GameComponent {
public:
	CollisionComponent();
	~CollisionComponent();

	virtual glm::vec3 checkCollision(std::shared_ptr<CollisionComponent> component) = 0;
	virtual glm::vec3 checkCollision(std::shared_ptr<CylinderComponent> component) = 0;
	virtual glm::mat4x4 getTransformMatrix() = 0;
	virtual std::shared_ptr<AABB> getAABB() = 0;
	virtual std::shared_ptr<AABB> getAABB(std::shared_ptr<Ray> ray) = 0;
	virtual void updateOnGround() = 0;

	std::shared_ptr<CollisionInfo> getEnvironmentClosestCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	std::shared_ptr<CollisionInfo> getEnvironmentClosestCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray, std::shared_ptr<BVH> bvh);

	std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> ellipsoidTriangleCollision(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> ellipsoidTriangleCollision(std::shared_ptr<BVH> bvh);

	glm::vec4 doNudge(glm::mat4x4& transformMatrix, glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents);
	glm::vec4 doNudge(glm::mat4x4& transformMatrix, glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::shared_ptr<BVH> bvh);

	std::weak_ptr<GridNode> gridNode;
	std::weak_ptr<CollisionSystem> collisionSystem;
};