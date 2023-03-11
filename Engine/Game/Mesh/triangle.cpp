#include "triangle.h"

const float epsilon = 0.00001;

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

CollisionInfo Triangle::intersect(glm::mat4x4 transformMatrix, const Ray& ray)
{
	auto res = CollisionInfo();

	// Transform the triangle to sphere space
	for (int i = 0; i < 3; i++)
		v[i]->setPosition(transformMatrix * v[i]->getPosition());
	calculateFaceNormal();

	// Check if the ray has intersection within the plane
	auto divisor = glm::dot(ray.direction, faceNormal);
	if (divisor > epsilon) {
		// Get intersection t
		res.t = glm::dot(this->v[0]->getPosition() - ray.origin, faceNormal) / divisor;
		// The triangle is in front of the ray
		if (res.t >= 0) {
			// Check Sphere-interior intersection
			if (isInside(ray.origin + res.t * ray.direction)) {
				Ray realRay(ray.origin - faceNormal, ray.direction);
				res.t = glm::dot(this->v[0]->getPosition() - realRay.origin, faceNormal) / divisor;
				res.contact = realRay.origin + res.t * realRay.direction;
			}
			// Check the other 2 intersections
			else {
				// Check Sphere-edge intersection

				// Check Sphere-Vertex intersection
			}
		}
	}

	// Transform the triangle back to world space
	for(int i = 0; i < 3; i++)
		v[i]->setPosition(glm::inverse(transformMatrix) * v[i]->getPosition());
	calculateFaceNormal();

	// Fill CollisionInfo
	res.normal = faceNormal;
	res.contact = glm::inverse(transformMatrix) * res.contact;

	return res;
}