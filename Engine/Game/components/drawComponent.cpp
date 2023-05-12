#include "Engine/Game/gameObject.h"
#include "drawComponent.h"
#include "transformComponent.h"

DrawComponent::DrawComponent() : GameComponent("draw")
{

}

DrawComponent::DrawComponent(std::string shapeType, std::string materialName, std::string materialNormalName) 
	: GameComponent("draw")
{
	shape = Global::graphics.getShape(shapeType);
	if (materialName.compare("") != 0) material = Global::graphics.getMaterial(materialName);
	if (materialNormalName.compare("") != 0) materialNormal = Global::graphics.getMaterial(materialNormalName);
}

void DrawComponent::drawPhong()
{
	std::shared_ptr<TransformComponent> transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	if (transformComponent == nullptr) return;
	//auto pos = transformComponent->getModelTransform()->getPos();
	//if (gameObject->getTag() == "character")
	//	std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	Global::graphics.setUniformFloat("alpha", alpha);
	Global::graphics.drawShape(shape, transformComponent->getModelTransform(), material, materialNormal);
}

void DrawComponent::setShape(std::string shapeType)
{
	shape = shape = Global::graphics.getShape(shapeType);
}

void DrawComponent::setMaterial(std::string materialName)
{
	if (materialName.compare("") != 0) material = Global::graphics.getMaterial(materialName);
}

float DrawComponent::getAlpha()
{
	return alpha;
}

void DrawComponent::setAlpha(float alpha)
{
	this->alpha = alpha;
}