#pragma once

#include <memory>

#include "Graphics/global.h"

#include <Graphics/shape.h>
#include <Graphics/material.h>

#include "Engine/Game/gameComponent.h"

class DrawComponent : public GameComponent {
public:
	DrawComponent();
	DrawComponent(std::string shapeType, std::string materialName);

	void draw();

	void setShape(std::string shapeType);
	void setMaterial(std::string materialName);

protected:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Material> material;
};