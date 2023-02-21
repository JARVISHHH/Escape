#include "Engine/Game/gameComponent.h"

#include "transformComponent.h"

TransformComponent::TransformComponent(std::string shapeType) : GameComponent("transform")
{
	shape = Global::graphics.getShape(shapeType);
	modelTransform = std::make_shared<ModelTransform>();
}

std::shared_ptr<Shape> TransformComponent::getShape()
{
	return shape;
}

std::shared_ptr<ModelTransform> TransformComponent::getModelTransform() {
	return modelTransform;
}