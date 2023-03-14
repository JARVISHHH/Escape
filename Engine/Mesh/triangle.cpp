#include "triangle.h"
#include <iostream>

const float epsilon = 0.00001f;

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

bool Triangle::intersectInterior(std::shared_ptr<Ray> ray, std::shared_ptr<CollisionInfo> collisionInfo)
{
	auto divisor = glm::dot(ray->direction, faceNormal);
	if (std::abs(divisor) > epsilon) {
		// Get intersection t
		collisionInfo->t = glm::dot(this->v[0]->getPosition() - (ray->origin - faceNormal), faceNormal) / divisor;
		// The triangle is in front of the ray
		if (collisionInfo->t >= 0) {
			// Check Sphere-interior intersection
			if (isInside(ray->origin - faceNormal + collisionInfo->t * ray->direction)) {
				collisionInfo->contact = ray->origin - faceNormal + collisionInfo->t * ray->direction;
				collisionInfo->center = collisionInfo->contact + faceNormal;
				return true;
			}
		}
	}
	return false;
}

void Triangle::intersectEdges(std::shared_ptr<Ray> ray, std::shared_ptr<CollisionInfo> collisionInfo)
{
	glm::vec3 A = glm::vec3(ray->origin), B = glm::vec3(ray->endPoint);
	for (int i = 0; i < 3; i++) {
		glm::vec3 C = glm::vec3(this->v[i]->getPosition()), D = glm::vec3(this->v[(i + 1) % 3]->getPosition());
		float a = std::pow(glm::length(glm::cross(B - A, D - C)), 2);
		float b = 2 * glm::dot(glm::cross(B - A, D - C), glm::cross(A - C, D - C));
		float c = std::pow(glm::length(glm::cross(A - C, D - C)), 2) - std::pow(glm::length(D - C), 2);

		// No intersection
		auto delta = b * b - 4 * a * c;
		if (delta < 0) continue;

		// Find the lesser value
		float t1 = (-b + std::sqrt(delta)) / (2 * a), t2 = (-b - std::sqrt(delta)) / (2 * a);
		float t = -1;
		if (t1 >= 0) t = t < 0 ? t1 : std::min(t, t1);
		if (t2 >= 0) t = t < 0 ? t2 : std::min(t, t2);
		if (t < 0) continue;

		// Check if in the line segment
		glm::vec3 P = glm::vec3(ray->origin + t * ray->direction);
		auto temp = glm::dot(P - C, D - C);
		if (temp > 0 && temp < std::pow(glm::length(D - C), 2)) {
			if (collisionInfo->t < 0 || t < collisionInfo->t) {
				collisionInfo->t = t;
				float project = glm::dot(P - C, glm::normalize(D - C));
				glm::vec3 contact = C + glm::normalize(D - C) * project;
				collisionInfo->contact = glm::vec4(contact[0], contact[1], contact[2], 1);
				collisionInfo->center = glm::vec4(P[0], P[1], P[2], 1);
			}
		}
	}
}

void Triangle::intersectVertices(std::shared_ptr<Ray> ray, std::shared_ptr<CollisionInfo> collisionInfo)
{
	for (int i = 0; i < 3; i++) {
		glm::vec3 origin = glm::vec3(this->v[i]->getPosition());
		float a = std::pow(glm::length(ray->direction), 2);
		float b = 2 * glm::dot(glm::vec3(-ray->direction), origin - glm::vec3(ray->origin));
		float c = std::pow(glm::length(origin - glm::vec3(ray->origin)), 2) - 1;
		float delta = b * b - 4 * a * c;
		if (delta < 0) continue;

		float t1 = (-b + std::sqrt(delta)) / (2 * a);
		float t2 = (-b - std::sqrt(delta)) / (2 * a);
		float t = -1;
		if (t1 >= 0) t = t < 0 ? t1 : std::min(t, t1);
		if (t2 >= 0) t = t < 0 ? t2 : std::min(t, t2);
		if (t < 0) continue;

		if (collisionInfo->t < 0 || t < collisionInfo->t) {
			collisionInfo->t = t;
			collisionInfo->contact = this->v[i]->getPosition();
			collisionInfo->center = collisionInfo->contact + ray->origin - (this->v[i]->getPosition() - t * ray->direction);
		}
	}
}

std::shared_ptr<CollisionInfo> Triangle::intersect(glm::mat4x4 transformMatrix, glm::mat4x4 triangleTransformMatrix, std::shared_ptr<Ray> ray)
{
	auto res = std::make_shared<CollisionInfo>();

	// Transform the triangle to sphere space
	for (int i = 0; i < 3; i++)
		v[i]->setPosition(transformMatrix * triangleTransformMatrix * v[i]->getPosition());
	calculateFaceNormal();

	if (!intersectInterior(ray, res)) {
		res->t = -1;
		// Check Sphere-edge intersection
		intersectEdges(ray, res);
		if (res->t >= 0) {
			for (int i = 0; i < 3; i++)
				v[i]->setPosition(glm::inverse(triangleTransformMatrix) * glm::inverse(transformMatrix) * v[i]->getPosition());
			calculateFaceNormal();

			res->contact = glm::inverse(transformMatrix) * res->contact;
			res->center = glm::inverse(transformMatrix) * res->center;
			res->normal = glm::normalize(res->center - res->contact);

			std::cout << "edges " << res->t << std::endl;

			return res;
		}
		// Check Sphere-Vertex intersection
		//intersectVertices(ray, res);
		//std::cout << "others" << std::endl;
	}

	// Transform the triangle back to world space
	for(int i = 0; i < 3; i++)
		v[i]->setPosition(glm::inverse(transformMatrix) * v[i]->getPosition());
	calculateFaceNormal();

	// Fill CollisionInfo
	res->normal = faceNormal;
	res->contact = glm::inverse(transformMatrix) * res->contact;
	res->center = glm::inverse(transformMatrix) * res->center;

	// Transform the triangle back to triangles space
	for (int i = 0; i < 3; i++)
		v[i]->setPosition(glm::inverse(triangleTransformMatrix) * v[i]->getPosition());
	calculateFaceNormal();

	return res;
}