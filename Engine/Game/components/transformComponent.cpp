#include "Engine/Game/gameComponent.h"

#include "transformComponent.h"

TransformComponent::TransformComponent() : GameComponent("transform")
{
	modelTransform = std::make_shared<ModelTransform>();
}

TransformComponent::~TransformComponent()
{
	std::cout << "transform component delete" << std::endl;
}

void TransformComponent::start()
{
	updateLastPos();
}

std::shared_ptr<ModelTransform> TransformComponent::getModelTransform() {
	return modelTransform;
}

std::shared_ptr<Ray> TransformComponent::getRay()
{
	return std::make_shared<Ray>(lastPos, modelTransform->getPos());
}

void TransformComponent::updateLastPos()
{
	lastPos = modelTransform->getPos();
}