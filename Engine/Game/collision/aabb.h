#pragma once

#include <memory>

#include <Engine/Game/components/collisionComponents/environmentComponent.h>

class AABB {
public:
    static bool compareX(std::shared_ptr<AABB> aabb1, std::shared_ptr<AABB> aabb2) {
        if (aabb1->minPoint.x == aabb2->minPoint.x) return aabb1->maxPoint.x < aabb2->maxPoint.x;
        return aabb1->minPoint.x < aabb2->minPoint.x;
    }

    static bool compareY(std::shared_ptr<AABB> aabb1, std::shared_ptr<AABB> aabb2) {
        if (aabb1->minPoint.y == aabb2->minPoint.y) return aabb1->maxPoint.y < aabb2->maxPoint.y;
        return aabb1->minPoint.y < aabb2->minPoint.y;
    }

    static bool compareZ(std::shared_ptr<AABB> aabb1, std::shared_ptr<AABB> aabb2) {
        if (aabb1->minPoint.z == aabb2->minPoint.z) return aabb1->maxPoint.z < aabb2->maxPoint.z;
        return aabb1->minPoint.z < aabb2->minPoint.z;
    }

    AABB(std::shared_ptr<EnvironmentComponent> environmentComponent, std::shared_ptr<Triangle> triangle);
    AABB(glm::vec4 maxPoint, glm::vec4 minPoint);

    glm::vec4 getCenter() {return center;}
    glm::vec4 getMaxPoint() { return maxPoint; }
    glm::vec4 getMinPoint() { return minPoint; }

    void setMaxPoint(glm::vec4 maxPoint) { this->maxPoint = maxPoint; }
    void setMinPoint(glm::vec4 minPoint) { this->minPoint = minPoint; }

    bool checkCollision(std::shared_ptr<AABB> movingAABB);
    std::shared_ptr<CollisionInfo> getCollision(glm::mat4x4& transformMatrix, std::shared_ptr<Ray> ray);

private:
    std::shared_ptr<EnvironmentComponent> environmentComponent;
    std::shared_ptr<Triangle> triangle;

    glm::vec4 center;
    glm::vec4 maxPoint;
    glm::vec4 minPoint;
};