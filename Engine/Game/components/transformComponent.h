#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/modeltransform.h>

#include "Engine/Game/gameComponent.h"

class TransformComponent : public GameComponent {
public:
	TransformComponent();
	std::shared_ptr<ModelTransform> getModelTransform();
	std::shared_ptr<Ray> getRay();

	void updateRay();
	void setRay(glm::vec3 origin = glm::vec3(0, 0, 0), glm::vec3 endPoint = glm::vec3(0, 0, 0));
	void setOnGround(bool onGround) { this->onGround = onGround; }
	bool isOnGround() { return onGround; }

protected:
	std::shared_ptr<ModelTransform> modelTransform;
	std::shared_ptr<Ray> ray;
	bool onGround = false;
};