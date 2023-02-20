#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/modeltransform.h>

#include "Engine/Game/gameComponent.h"

class TransformComponent : public GameComponent {
public:
	TransformComponent();
	std::shared_ptr<ModelTransform> getModelTransform();

protected:
	std::shared_ptr<ModelTransform> modelTransform;
};