#include "collisionComponent.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/collision/gridnode.h>

#include <Engine/Game/components/collisionComponents/cylinderComponent.h>

const float EPSILON = 0.0000001;

CollisionComponent::CollisionComponent()
	:GameComponent("collision")
{

}

CollisionComponent::CollisionComponent(std::shared_ptr<ModelTransform> modelTransform)
	:GameComponent("collision"), modelTransform(modelTransform)
{
}

CollisionComponent::~CollisionComponent()
{
	//std::cout << "collisionComponent delete" << std::endl;
}

std::shared_ptr<AABB> CollisionComponent::getAABB(std::shared_ptr<ModelTransform> modelTransform) {
	std::vector<glm::vec4> points(8);
	points[0] = { 0.5, 0.5, 0.5, 1 };
	points[1] = { -0.5, 0.5, 0.5, 1 };
	points[2] = { -0.5, 0.5, -0.5, 1 };
	points[3] = { 0.5, 0.5, -0.5, 1 };
	points[4] = { 0.5, -0.5, 0.5, 1 };
	points[5] = { -0.5, -0.5, 0.5, 1 };
	points[6] = { -0.5, -0.5, -0.5, 1 };
	points[7] = { 0.5, -0.5, -0.5, 1 };

	for (int i = 0; i < 8; i++) {
		points[i] = modelTransform->getModelMatrix() * points[i];
	}

	auto maxPoint = points[0], minPoint = points[0];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], points[i][j]);
			minPoint[j] = std::min(minPoint[j], points[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

// For environment collision
std::shared_ptr<AABB> CollisionComponent::getAABB(glm::mat4x4 inverseTransformMatrix) {
	std::vector<glm::vec4> points(8);
	points[0] = { 1, 1, 1, 1 };
	points[1] = { -1, 1, 1, 1 };
	points[2] = { -1, 1, -1, 1 };
	points[3] = { 1, 1, -1, 1 };
	points[4] = { 1, -1, 1, 1 };
	points[5] = { -1, -1, 1, 1 };
	points[6] = { -1, -1, -1, 1 };
	points[7] = { 1, -1, -1, 1 };

	auto transformMatrix = glm::inverse(inverseTransformMatrix);

	for (int i = 0; i < 8; i++) {
		points[i] = transformMatrix * points[i];
	}

	auto maxPoint = points[0], minPoint = points[0];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], points[i][j]);
			minPoint[j] = std::min(minPoint[j], points[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

std::shared_ptr<AABB> CollisionComponent::getAABB(std::shared_ptr<ModelTransform> modelTransform, std::shared_ptr<Ray> ray)
{
	std::vector<glm::vec4> points(8);
	std::vector<glm::vec4> worldPoints(16);
	points[0] = { 0.5, 0.5, 0.5, 1 };
	points[1] = { -0.5, 0.5, 0.5, 1 };
	points[2] = { -0.5, 0.5, -0.5, 1 };
	points[3] = { 0.5, 0.5, -0.5, 1 };
	points[4] = { 0.5, -0.5, 0.5, 1 };
	points[5] = { -0.5, -0.5, 0.5, 1 };
	points[6] = { -0.5, -0.5, -0.5, 1 };
	points[7] = { 0.5, -0.5, -0.5, 1 };

	for (int i = 0; i < 8; i++) {
		worldPoints[i] = modelTransform->getModelMatrix() * points[i] + ray->direction;
	}
	for (int i = 8; i < 16; i++) {
		worldPoints[i] = modelTransform->getModelMatrix() * points[i - 8];
	}

	auto maxPoint = worldPoints[0], minPoint = worldPoints[0];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], worldPoints[i][j]);
			minPoint[j] = std::min(minPoint[j], worldPoints[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

// For environment collision
std::shared_ptr<AABB> CollisionComponent::getAABB(glm::mat4x4 inverseTransformMatrix, std::shared_ptr<Ray> ray)
{
	std::vector<glm::vec4> points(8);
	std::vector<glm::vec4> worldPoints(16);
	points[0] = { 1, 1, 1, 1 };
	points[1] = { -1, 1, 1, 1 };
	points[2] = { -1, 1, -1, 1 };
	points[3] = { 1, 1, -1, 1 };
	points[4] = { 1, -1, 1, 1 };
	points[5] = { -1, -1, 1, 1 };
	points[6] = { -1, -1, -1, 1 };
	points[7] = { 1, -1, -1, 1 };

	auto transformMatrix = glm::inverse(inverseTransformMatrix);

	for (int i = 0; i < 8; i++) {
		worldPoints[i] = transformMatrix * points[i] + ray->direction;
	}
	for (int i = 8; i < 16; i++) {
		worldPoints[i] = transformMatrix * points[i - 8];
	}

	auto maxPoint = worldPoints[0], minPoint = worldPoints[0];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(maxPoint[j], worldPoints[i][j]);
			minPoint[j] = std::min(minPoint[j], worldPoints[i][j]);
		}
	}

	return std::make_shared<AABB>(maxPoint, minPoint);
}

std::shared_ptr<ModelTransform> CollisionComponent::getTransform()
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	std::shared_ptr<ModelTransform> transform = std::make_shared<ModelTransform>();
	transform->copy(modelTransform);
	transform->transform(transformComponent->getModelTransform());

	return transform;
}

glm::mat4x4 CollisionComponent::getTransformMatrix()
{
	auto transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	return transformComponent->getModelTransform()->getModelMatrix() * modelTransform->getModelMatrix();
}

std::shared_ptr<Ray> CollisionComponent::getRay()
{
	auto ray = getGameObject()->getComponent<TransformComponent>("transform")->getRay();
	glm::vec4 collisionOrigin, collisionEnd;
	auto objectModelTransform = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform();
	{
		auto objectTransformMatrix = objectModelTransform->getModelMatrix();
		auto objectInverse = glm::inverse(objectTransformMatrix);
		auto totalMatrix = objectTransformMatrix * modelTransform->getModelMatrix() * objectInverse;
		collisionEnd = totalMatrix * ray->endPoint;
		//for (int i = 0; i < 4; i++) {
			//for (int j = 0; j < 4; j++)
				//std::cout << modelTransform->getModelMatrix()[i][j] << " ";
			//std::cout << std::endl;
		//}
		//auto temp = objectInverse * ray->endPoint;
		//std::cout << "temp: " << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << std::endl;
		//std::cout << "object pos: " << ray->endPoint[0] << " " << ray->endPoint[1] << " " << ray->endPoint[2] << " " << ray->endPoint[3] << std::endl;
		//std::cout << "collision pos: " << collisionEnd[0] << " " << collisionEnd[1] << " " << collisionEnd[2] << " " << collisionEnd[3] << std::endl;
	}
	{
		auto tempModelTransform = std::make_shared<ModelTransform>();
		tempModelTransform->copy(objectModelTransform);
		tempModelTransform->translate(ray->origin - ray->endPoint);
		auto objectTransformMatrix = tempModelTransform->getModelMatrix();
		auto objectInverse = glm::inverse(objectTransformMatrix);
		auto totalMatrix = objectTransformMatrix * modelTransform->getModelMatrix() * objectInverse;
		collisionOrigin = totalMatrix * ray->origin;
		//std::cout << "object pos: " << ray->origin[0] << " " << ray->origin[1] << " " << ray->origin[2] << std::endl;
		//std::cout << "collision pos: " << collisionOrigin[0] << " " << collisionOrigin[1] << " " << collisionOrigin[2] << std::endl;
	}
	ray->update(collisionOrigin, collisionEnd);
	return ray;
}

std::shared_ptr<CollisionInfo> CollisionComponent::getEnvironmentClosestCollision(glm::mat4x4 inverseTransformMatrix, std::shared_ptr<Ray> ray, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto res = std::make_shared<CollisionInfo>();

	auto sphereSpaceRay = std::make_shared<Ray>(inverseTransformMatrix * ray->origin, inverseTransformMatrix * ray->endPoint);

	for (auto environmentComponent : environmentComponents) {
		auto thisCollision = environmentComponent->getClosestCollision(inverseTransformMatrix, sphereSpaceRay);
		if (thisCollision->t >= 0 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;
	}

	return res;
}

std::shared_ptr<CollisionInfo> CollisionComponent::getEnvironmentClosestCollision(glm::mat4x4 inverseTransformMatrix, std::shared_ptr<Ray> ray, std::shared_ptr<BVH> bvh)
{
	auto res = std::make_shared<CollisionInfo>();

	//auto movingAABB = getAABB(ray);
	auto movingAABB = CollisionComponent::getAABB(inverseTransformMatrix, ray);
	auto sphereSpaceRay = std::make_shared<Ray>(inverseTransformMatrix * ray->origin, inverseTransformMatrix * ray->endPoint);
	

	//std::cout << "sphereSpaceRay: " << sphereSpaceRay->origin[0] << " " << sphereSpaceRay->origin[1] << " " << sphereSpaceRay->origin[2] << std::endl;

	auto thisCollision = bvh->getClosestCollision(inverseTransformMatrix, movingAABB, sphereSpaceRay);
	if (thisCollision != nullptr && thisCollision->t >= 0 && thisCollision->t <= 1 && (thisCollision->t < res->t || res->t < 0)) res = thisCollision;

	return res;
}

std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> CollisionComponent::ellipsoidTriangleCollision(std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto ray = getRay();

	if (glm::length(ray->direction) == 0) return std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3>();

	//std::cout << std::endl;
	//std::cout << "start from: " << ray->origin[0] << " " << ray->origin[1] << " " << ray->origin[2] << std::endl;
	//std::cout << "end to: " << ray->endPoint[0] << " " << ray->endPoint[1] << " " << ray->endPoint[2] << std::endl;

	std::vector<std::shared_ptr<CollisionInfo>> collisions;
	glm::mat4x4 inverseTransformMatrix = getInverseTransformMatrix(false, ray->origin);
	auto initialPos = ray->origin, finalPos = ray->endPoint;
	auto curPos = initialPos, nextPos = finalPos;

	for (int i = 0; i < 3; i++) {
		auto c = getEnvironmentClosestCollision(inverseTransformMatrix, std::make_shared<Ray>(initialPos, nextPos), environmentComponents);
		//std::cout << "iteration: " << i << std::endl;
		//if(c->t > 0) std::cout << "normal: " << c->normal[0] << " " << c->normal[1] << " " << c->normal[2] << std::endl;
		//std::cout << "t: " << c->t << std::endl;
		if (c->t < 0 || c->t > 1) {
			//std::cout << "iteration: " << i << " after collision : " << nextPos[0] << " " << nextPos[1] << " " << nextPos[2] << std::endl;
			curPos = nextPos;
			break;
		}
		//curPos = c->center;
		curPos = doNudge(curPos, c, environmentComponents);
		auto d = nextPos - curPos;
		auto dCorrected = d - glm::dot(d, c->normal) * c->normal;
		nextPos = curPos + dCorrected;
		collisions.push_back(c);
	}

	auto objectTransformMatrix = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();
	auto collisionTrnasformMatrix = modelTransform->getModelMatrix();
	auto pos = objectTransformMatrix * glm::inverse(collisionTrnasformMatrix) * glm::inverse(objectTransformMatrix) * curPos;

	return { collisions, pos };
}

std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3> CollisionComponent::ellipsoidTriangleCollision(std::shared_ptr<BVH> bvh)
{
	auto ray = getRay();

	if (glm::length(ray->direction) == 0) return std::pair<std::vector<std::shared_ptr<CollisionInfo>>, glm::vec3>();

	auto start = ray->origin;
	auto end = ray->endPoint;
	//std::cout << "start: " << start[0] << " " << start[1] << " " << start[2] << std::endl;
	//std::cout << "end: " << end[0] << " " << end[1] << " " << end[2] << std::endl;

	std::vector<std::shared_ptr<CollisionInfo>> collisions;
	glm::mat4x4 inverseTransformMatrix = getInverseTransformMatrix(false, ray->origin);
	auto initialPos = ray->origin, finalPos = ray->endPoint;
	auto curPos = initialPos, nextPos = finalPos;

	for (int i = 0; i < 5; i++) {
		auto c = getEnvironmentClosestCollision(inverseTransformMatrix, std::make_shared<Ray>(initialPos, nextPos), bvh);
		if (c->t <= 0 || c->t >= 1) {
			//if (collisions.size() == 0) std::cout << "no collsion" << std::endl;
			//else std::cout << "collision" << std::endl;
			curPos = nextPos;
			break;
		}
		curPos = doNudge(curPos, c, bvh);
		auto d = nextPos - curPos;
		auto dCorrected = d - glm::dot(d, c->normal) * c->normal;
		nextPos = curPos + dCorrected;
		collisions.push_back(c);
	}

	//if (collisions.size() == 0) std::cout << "no collsion" << std::endl;
	//else std::cout << "collision" << std::endl;

	auto objectTransformMatrix = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getModelMatrix();
	auto collisionTrnasformMatrix = modelTransform->getModelMatrix();
	auto pos = objectTransformMatrix * glm::inverse(collisionTrnasformMatrix) * glm::inverse(objectTransformMatrix) * curPos;

	return { collisions, pos };
}

glm::vec4 CollisionComponent::doNudge(glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::vector<std::shared_ptr<EnvironmentComponent>>& environmentComponents)
{
	auto nudge = collision->normal;
	auto nudgedPos = collision->center + nudge * 0.001f;
	for (int i = 0; i < 3; i++) {
		//std::cout << "nudge check" << std::endl;
		glm::mat4x4 inverseTransformMatrix = getInverseTransformMatrix(false, curPos);
		auto nudgeCollision = getEnvironmentClosestCollision(inverseTransformMatrix, std::make_shared<Ray>(curPos, nudgedPos), environmentComponents);
		//std::cout << "nudge finish" << std::endl;
		if (nudgeCollision->t < 0 || nudgeCollision->t > 1) {
			curPos = nudgedPos;
			break;
		}
		//std::cout << "nudge" << std::endl;
		if (glm::length(nudgeCollision->normal - nudge) < EPSILON || glm::length(nudgeCollision->normal + nudge) < EPSILON) nudge = -nudgeCollision->normal;
		else nudge = nudgeCollision->normal;
		nudgedPos = nudgeCollision->center + nudge * 0.001f;
	}

	return curPos;
}

glm::vec4 CollisionComponent::doNudge(glm::vec4 curPos, std::shared_ptr<CollisionInfo> collision, std::shared_ptr<BVH> bvh)
{
	auto nudge = collision->normal;
	auto nudgedPos = collision->center + nudge * 0.001f;
	for (int i = 0; i < 3; i++) {
		//std::cout << "nudge check" << std::endl;
		glm::mat4x4 inverseTransformMatrix = getInverseTransformMatrix(false, curPos);
		auto nudgeCollision = getEnvironmentClosestCollision(inverseTransformMatrix, std::make_shared<Ray>(curPos, nudgedPos), bvh);
		//std::cout << "nudge finish" << std::endl;
		if (nudgeCollision->t < 0 || nudgeCollision->t > 1) {
			curPos = nudgedPos;
			break;
		}
		//std::cout << "nudge" << std::endl;
		if (glm::length(nudgeCollision->normal - nudge) < EPSILON || glm::length(nudgeCollision->normal + nudge) < EPSILON) nudge = -nudgeCollision->normal;
		else nudge = nudgeCollision->normal;
		nudgedPos = nudgeCollision->center + nudge * 0.001f;
	}

	return curPos;
}