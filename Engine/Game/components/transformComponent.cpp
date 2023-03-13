#include "Engine/Game/gameComponent.h"

#include "transformComponent.h"

TransformComponent::TransformComponent() : GameComponent("transform")
{
	modelTransform = std::make_shared<ModelTransform>();
	ray = std::make_shared<Ray>();
}

std::shared_ptr<ModelTransform> TransformComponent::getModelTransform() {
	return modelTransform;
}

std::shared_ptr<Ray> TransformComponent::getRay()
{
	return ray;
}

void TransformComponent::updateRay() {
	ray->update(ray->endPoint, modelTransform->getPos());
	//std::cout << "start: " << ray->origin[0] << " " << ray->origin[1] << " " << ray->origin[2] << " " << ray->origin[3] << std::endl;
	//std::cout << "end: " << ray->endPoint[0] << " " << ray->endPoint[1] << " " << ray->endPoint[2] << " " << ray->endPoint[3] << std::endl;
}

void TransformComponent::setRay(glm::vec3 origin, glm::vec3 endPoint)
{
	ray->update(origin, endPoint);
}