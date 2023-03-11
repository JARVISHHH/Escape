#include "triangle.h"

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

CollisionInfo Triangle::intersect(glm::mat4x4 transformMatrix, const Ray& ray)
{

	return CollisionInfo();
}