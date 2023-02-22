#include "Engine/Game/gameComponent.h"

#include "transformComponent.h"

TransformComponent::TransformComponent() : GameComponent("transform")
{
	modelTransform = std::make_shared<ModelTransform>();
}

std::shared_ptr<ModelTransform> TransformComponent::getModelTransform() {
	return modelTransform;
}