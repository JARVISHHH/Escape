#include "collisionComponent.h"
#include <Engine/Game/components/transformComponent.h>

const float EPSILON = 0.0005;

CollisionComponent::CollisionComponent()
	:GameComponent("collision")
{

}

std::shared_ptr<CollisionInfo> CollisionComponent::getEnvironmentClosestCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto res = std::make_shared<CollisionInfo>();

	auto sphereSpaceRay = std::make_shared<Ray>(transformMatrix * ray->origin, transformMatrix * ray->endPoint);

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

	//std::cout << std::endl;
	//std::cout << "start from: " << ray->origin[0] << " " << ray->origin[1] << " " << ray->origin[2] << std::endl;
	//std::cout << "end to: " << ray->endPoint[0] << " " << ray->endPoint[1] << " " << ray->endPoint[2] << std::endl;

	std::vector<std::shared_ptr<CollisionInfo>> collisions;
	glm::mat4x4 transformMatrix = getTransformMatrix();
	glm::mat4x4 inverseMatrix = glm::inverse(transformMatrix);
	auto curPos = ray->origin, nextPos = ray->endPoint;

	for (int i = 0; i < 3; i++) {
		auto c = getEnvironmentClosestCollision(transformMatrix, std::make_shared<Ray>(curPos, nextPos), environmentComponents);
		//std::cout << "iteration: " << i << std::endl;
		//if(c->t > 0) std::cout << "normal: " << c->normal[0] << " " << c->normal[1] << " " << c->normal[2] << std::endl;
		//std::cout << "t: " << c->t << std::endl;
		if (c->t < 0 || c->t > 1) {
			//std::cout << "iteration: " << i << " after collision : " << nextPos[0] << " " << nextPos[1] << " " << nextPos[2] << std::endl;
			return { collisions, nextPos };
		}
		//curPos = c->center;
		curPos = doNudge(transformMatrix, curPos, c, environmentComponents);
		auto d = nextPos - curPos;
		auto dCorrected = d - glm::dot(d, c->normal) * c->normal;
		nextPos = curPos + dCorrected;
		collisions.push_back(c);
	}

	return { collisions, curPos };
}

glm::vec4 CollisionComponent::doNudge(glm::mat4x4& transformMatrix, glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto nudge = collision->normal;
	auto nudgedPos = collision->center + nudge * 0.005f;
	for (int i = 0; i < 3; i++) {
		//std::cout << "nudge check" << std::endl;
		auto nudgeCollision = getEnvironmentClosestCollision(transformMatrix, std::make_shared<Ray>(curPos, nudgedPos), environmentComponents);
		//std::cout << "nudge finish" << std::endl;
		if (nudgeCollision->t < 0 || nudgeCollision->t > 1) {
			curPos = nudgedPos;
			break;
		}
		//std::cout << "nudge" << std::endl;
		if (glm::length(nudgeCollision->normal - nudge) < EPSILON || glm::length(nudgeCollision->normal + nudge) < EPSILON) nudge = -nudgeCollision->normal;
		else nudge = nudgeCollision->normal;
		nudgedPos = nudgeCollision->center + nudge * 0.005f;
	}

	return curPos;
}