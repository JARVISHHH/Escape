#include "Engine/Game/gameObject.h"
#include "drawComponent.h"
#include "transformComponent.h"

DrawComponent::DrawComponent() : GameComponent("draw")
{

}

DrawComponent::DrawComponent(std::string shapeType) : GameComponent("draw")
{
	shape = Global::graphics.getShape(shapeType);
}

DrawComponent::DrawComponent(std::string shapeType, std::string materialName) : GameComponent("draw")
{
	shape = Global::graphics.getShape(shapeType);
	if(materialName.compare("") != 0) material = Global::graphics.getMaterial(materialName);
}

void DrawComponent::draw()
{
	std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (transformComponent == nullptr) return;
	Global::graphics.drawShape(shape, transformComponent->getModelTransform(), material);
}

void DrawComponent::setShape(std::string shapeType)
{
	shape = shape = Global::graphics.getShape(shapeType);
}

void DrawComponent::setMaterial(std::string materialName)
{
	if (materialName.compare("") != 0) material = Global::graphics.getMaterial(materialName);
}