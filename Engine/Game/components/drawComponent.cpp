#include "Engine/Game/gameObject.h"
#include "drawComponent.h"
#include "transformComponent.h"

DrawComponent::DrawComponent() : GameComponent("draw")
{

}

DrawComponent::DrawComponent(std::string materialName) : GameComponent("draw")
{
	material = Global::graphics.getMaterial(materialName);
}

void DrawComponent::draw()
{
	std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (transformComponent == nullptr) return;
	Global::graphics.drawShape(transformComponent->getShape(), transformComponent->getModelTransform(), material);
}

void DrawComponent::setMaterial(std::string materialName)
{
	material = Global::graphics.getMaterial(materialName);
}