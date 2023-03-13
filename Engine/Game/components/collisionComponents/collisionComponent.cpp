#include "collisionComponent.h"
#include <Engine/Game/components/transformComponent.h>

const float EPSILON = 0.0005;

CollisionComponent::CollisionComponent()
	:GameComponent("collision")
{

}

std::shared_ptr<CollisionInfo> CollisionComponent::getEnvironmentClosestCollision(glm::mat4x4 transformMatrix, std::shared_ptr<Ray> ray, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto res = std::make_shared<CollisionInfo>();

	//transformMatrix = glm::inverse(gameObject->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix());

	auto sphereSpaceRay = std::make_shared<Ray>(transformMatrix * ray->origin, transformMatrix * ray->endPoint);

	//std::cout << "origin: " << sphereSpaceRay->origin[0] << " " << sphereSpaceRay->origin[1] << " " << sphereSpaceRay->origin[2] << " " << sphereSpaceRay->origin[3] << std::endl;

	for (auto environmentComponent : environmentComponents) {
		auto thisCollision = environmentComponent->getClosestCollision(transformMatrix, sphereSpaceRay);
		if (thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	}

	return res;
}

std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> CollisionComponent::ellipsoidTriangleCollision(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto ray = gameObject->getComponent<TransformComponent>("transform")->getRay();
	if (glm::length(ray->direction) < EPSILON) return std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3>();

	std::vector<std::shared_ptr<CollisionInfo>> collisions;
	glm::mat4x4 transformMatrix = getTransformMatrix();
	glm::mat4x4 inverseMatrix = glm::inverse(transformMatrix);
	auto curPos = ray->origin, nextPos = ray->endPoint;

	for (int i = 0; i < 3; i++) {
		auto c = getEnvironmentClosestCollision(transformMatrix, std::make_shared<Ray>(curPos, nextPos), environmentComponents);
		if(c->t >= 0) std::cout << c->t << std::endl;
		if (c->t <= 1 && c->t >= 0) std::cout << "collide" << std::endl;
		if (c->t < 0 || c->t > 1) return { collisions, nextPos };
		curPos = doNudge(transformMatrix, curPos, c, environmentComponents);
		auto d = nextPos - curPos;
		auto dCorrected = d - glm::dot(d, c->normal) * c->normal;
		nextPos = curPos + dCorrected;
		collisions.push_back(c);
	}

	return { collisions, curPos };
}

glm::vec4 CollisionComponent::doNudge(glm::mat4x4 transformMatrix, glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto nudge = collision->normal;
	auto nudgedPos = collision->contact + nudge * 0.1f;
	for (int i = 0; i < 3; i++) {
		auto nudgeCollision = getEnvironmentClosestCollision(transformMatrix, std::make_shared<Ray>(curPos, nudgedPos), environmentComponents);
		if (nudgeCollision->t < 0 || nudgeCollision->t > 1) return nudgedPos;
		if (glm::length(nudgeCollision->normal - nudge) < EPSILON || glm::length(nudgeCollision->normal + nudge) < EPSILON) nudge = -nudgeCollision->normal;
		else nudge = nudgeCollision->normal;
	}

	return curPos;
}