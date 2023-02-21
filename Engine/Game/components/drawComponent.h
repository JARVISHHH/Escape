#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/shape.h>
#include <Graphics/material.h>

#include "Engine/Game/gameComponent.h"

class DrawComponent : public GameComponent {
public:
	DrawComponent();
	DrawComponent(std::string materialName);

	void draw();

	void setMaterial(std::string materialName);

protected:
	std::shared_ptr<Material> material;
};