#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/modeltransform.h>

#include "Engine/Game/gameComponent.h"

class TransformComponent : public GameComponent {
public:
	TransformComponent(std::string shapeType);
	std::shared_ptr<Shape> getShape();
	std::shared_ptr<ModelTransform> getModelTransform();

protected:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<ModelTransform> modelTransform;
};