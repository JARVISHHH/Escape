#include "Engine/Game/gameObject.h"
#include "drawComponent.h"
#include "transformComponent.h"

DrawComponent::DrawComponent() : GameComponent()
{

}

void DrawComponent::draw()
{
	std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>("transform");
	//if (transformComponent == nullptr) return;
	//Global::graphics.drawShape(shape, transformComponent->getModelTransform(), material);
}

void DrawComponent::setShape(std::string shapeType)
{
	shape = Global::graphics.getShape(shapeType);
}

void DrawComponent::setMaterial(std::string materialName)
{
	material = Global::graphics.getMaterial(materialName);
}