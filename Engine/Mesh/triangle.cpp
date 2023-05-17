#include "triangle.h"
#include <iostream>

const float epsilon = 0.00000001f;

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	this->v = std::vector<std::shared_ptr<Vertex>>(3);
	this->v[0] = std::make_shared<Vertex>(v1);
	this->v[1] = std::make_shared<Vertex>(v2);
	this->v[2] = std::make_shared<Vertex>(v3);

	calculateFaceNormal();
}

Triangle::Triangle(std::shared_ptr<Vertex>* v)
{
	this->v = std::vector<std::shared_ptr<Vertex>>(3);
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];

	calculateFaceNormal();
}

glm::vec4 Triangle::calculateFaceNormal()
{
	auto a = v[1]->getPosition() - v[0]->getPosition(), b = v[2]->getPosition() - v[0]->getPosition();
	auto normal = glm::normalize(glm::cross(glm::vec3(a), glm::vec3(b)));
	faceNormal = glm::vec4(normal[0], normal[1], normal[2], 0);
	return faceNormal;
}

bool Triangle::isInside(glm::vec4 pos)
{
	std::vector<float> products(3);
	for (int i = 0; i < 3; i++) {
		products[i] = glm::dot(glm::cross(glm::vec3(this->v[(i + 1) % 3]->getPosition() - this->v[i]->getPosition()), glm::vec3(pos - this->v[i]->getPosition())), glm::vec3(faceNormal));
	}
	if (products[0] * products[1] <= 0 || products[0] * products[2] <= 0) return false;
	return true;
}

bool Triangle::isInside(std::vector<glm::vec4>& sphereSpacePos, glm::vec4 pos)
{
	std::vector<float> products(3);
	for (int i = 0; i < 3; i++) {
		products[i] = glm::dot(glm::cross(glm::vec3(sphereSpacePos[(i + 1) % 3] - sphereSpacePos[i]), glm::vec3(pos - sphereSpacePos[i])), glm::vec3(faceNormal));
	}
	if (products[0] * products[1] <= 0 || products[0] * products[2] <= 0) return false;
	return true;
}

bool Triangle::intersectInterior(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo)
{
	auto divisor = glm::dot(sphereSpaceRay->direction, faceNormal);
	if (std::abs(divisor) > epsilon) {
		if (glm::dot(glm::normalize(sphereSpaceRay->direction), faceNormal) > 0) return false;
		// Get intersection t
		collisionInfo->t = glm::dot(this->v[0]->getPosition() - (sphereSpaceRay->origin - faceNormal), faceNormal) / divisor;
		// The triangle is in front of the ray
		if (collisionInfo->t >= 0 && collisionInfo->t <= 1) {
			// Check Sphere-interior intersection
			if (isInside(sphereSpaceRay->origin - faceNormal + collisionInfo->t * sphereSpaceRay->direction)) {
				collisionInfo->contact = sphereSpaceRay->origin - faceNormal + collisionInfo->t * sphereSpaceRay->direction;
				collisionInfo->center = collisionInfo->contact + faceNormal;
				return true;
			}
		}
	}
	return false;
}

bool Triangle::intersectEdges(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo)
{
	bool closer = false;
	glm::vec3 A = glm::vec3(sphereSpaceRay->origin), B = glm::vec3(sphereSpaceRay->endPoint);
	for (int i = 0; i < 3; i++) {
		glm::vec3 D = glm::vec3(this->v[i]->getPosition()), C = glm::vec3(this->v[(i + 1) % 3]->getPosition());
		float a = std::pow(glm::length(glm::cross(B - A, D - C)), 2);
		float b = 2 * glm::dot(glm::cross(B - A, D - C), glm::cross(A - C, D - C));
		float c = std::pow(glm::length(glm::cross(A - C, D - C)), 2) - std::pow(glm::length(D - C), 2);

		// No intersection
		auto delta = b * b - 4 * a * c;
		if (delta < 0) continue;

		// Find the lesser value
		float t1 = (-b + std::sqrt(delta)) / (2 * a), t2 = (-b - std::sqrt(delta)) / (2 * a);
		float t = std::min(t1, t2);
		//if (t1 >= 0) t = t < 0 ? t1 : std::min(t, t1);
		//if (t2 >= 0) t = t < 0 ? t2 : std::min(t, t2);
		if (t < 0 || t > 1) continue;

		// Check if in the line segment
		glm::vec3 P = glm::vec3(A + t * (B - A));
		auto temp = glm::dot(P - C, D - C);
		if (temp > 0 && temp < std::pow(glm::length(D - C), 2)) {
			if (collisionInfo->t < 0 || t < collisionInfo->t) {
				closer = true;
				collisionInfo->t = t;
				float cosine = glm::dot(glm::normalize(P - C), glm::normalize(D - C));
				glm::vec3 contact = C + glm::normalize(D - C) * cosine * glm::length(P - C);
				collisionInfo->contact = glm::vec4(contact[0], contact[1], contact[2], 1);
				collisionInfo->center = glm::vec4(P[0], P[1], P[2], 1);
			}
		}
	}
	
	return closer;
}

bool Triangle::intersectVertices(std::shared_ptr<Ray> sphereSpaceRay, std::shared_ptr<CollisionInfo> collisionInfo)
{
	bool closer = false;
	for (int i = 0; i < 3; i++) {
		glm::vec3 origin = glm::vec3(this->v[i]->getPosition());
		float a = glm::dot(-sphereSpaceRay->direction, -sphereSpaceRay->direction);
		float b = 2 * glm::dot(glm::vec3(-sphereSpaceRay->direction), origin - glm::vec3(sphereSpaceRay->origin));
		float c = glm::dot(origin - glm::vec3(sphereSpaceRay->origin), origin - glm::vec3(sphereSpaceRay->origin)) - 1;
		float delta = b * b - 4 * a * c;
		if (delta < 0) continue;

		float t1 = (-b + std::sqrt(delta)) / (2 * a);
		float t2 = (-b - std::sqrt(delta)) / (2 * a);
		float t = std::min(t1, t2);
		//if (t1 >= 0) t = t < 0 ? t1 : std::min(t, t1);
		//if (t2 >= 0) t = t < 0 ? t2 : std::min(t, t2);
		if (t < 0 || t > 1) continue;

		if (collisionInfo->t < 0 || t < collisionInfo->t) {
			closer = true;
			collisionInfo->t = t;
			collisionInfo->contact = this->v[i]->getPosition();
			collisionInfo->center = collisionInfo->contact + sphereSpaceRay->origin - (this->v[i]->getPosition() - t * sphereSpaceRay->direction);
		}
	}
	
	return closer;
}

// transformMatrix world space -> sphere space
// triangleTransformMatrix triangle space -> world space
std::shared_ptr<CollisionInfo> Triangle::intersect(glm::mat4x4 inverseTransformMatrix, glm::mat4x4 triangleTransformMatrix, std::shared_ptr<Ray> sphereSpaceRay)
{
	auto res = std::make_shared<CollisionInfo>();

	// Transform the triangle to sphere space
	for (int i = 0; i < 3; i++)
		v[i]->setPosition(inverseTransformMatrix * triangleTransformMatrix * v[i]->getPosition());
	calculateFaceNormal();

	// Sphere-interior collision
	if (intersectInterior(sphereSpaceRay, res)) {
		// Transform the triangle back to world space
		for (int i = 0; i < 3; i++)
			v[i]->setPosition(glm::inverse(inverseTransformMatrix) * v[i]->getPosition());
		calculateFaceNormal();

		// Fill CollisionInfo
		res->normal = faceNormal;
		res->contact = glm::inverse(inverseTransformMatrix) * res->contact;
		res->center = glm::inverse(inverseTransformMatrix) * res->center;

		// Transform the triangle back to triangles space
		for (int i = 0; i < 3; i++)
			v[i]->setPosition(glm::inverse(triangleTransformMatrix) * v[i]->getPosition());
		calculateFaceNormal();

		return res;
	}
	else 
	{
		res->t = -1;
		//std::cout << "try edges" << std::endl;
		// Sphere-edges collision
		if (intersectEdges(sphereSpaceRay, res)) {
			res->contact = glm::inverse(inverseTransformMatrix) * res->contact;
			res->center = glm::inverse(inverseTransformMatrix) * res->center;
			res->normal = glm::normalize(res->center - res->contact);
			//std::cout << "collide with edges: " << res->center[0] << " " << res->center[1] << " " << res->center[2] << " " << std::endl;
		}
		// Sphere-vertices collision
		if (intersectVertices(sphereSpaceRay, res)) {
			res->contact = glm::inverse(inverseTransformMatrix) * res->contact;
			res->center = glm::inverse(inverseTransformMatrix) * res->center;
			res->normal = glm::normalize(res->center - res->contact);
		}

		// Transform the triangle back to triangles space
		for (int i = 0; i < 3; i++)
			v[i]->setPosition(glm::inverse(triangleTransformMatrix) * glm::inverse(inverseTransformMatrix) * v[i]->getPosition());
		calculateFaceNormal();

		return res;
	}
}