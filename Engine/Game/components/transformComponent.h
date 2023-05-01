#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/modeltransform.h>

#include "Engine/Game/gameComponent.h"

class TransformComponent : public GameComponent {
public:
	TransformComponent();
	~TransformComponent();

	void start() override;

	std::shared_ptr<ModelTransform> getModelTransform();
	std::shared_ptr<Ray> getRay();

	void updateLastPos();
	void setOnGround(bool onGround) { this->onGround = onGround; }
	bool isOnGround() { return onGround; }

protected:
	std::shared_ptr<ModelTransform> modelTransform;
	glm::vec3 lastPos;
	bool onGround = false;
};