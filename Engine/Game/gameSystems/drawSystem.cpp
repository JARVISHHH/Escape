#include "Engine/Game/gameObject.h"
#include "drawSystem.h"
#include "Engine/Game/components/drawComponent.h"

DrawSystem::DrawSystem() : GameSystem("draw")
{
	
}

DrawSystem::DrawSystem(std::shared_ptr<GameWorld> gameWorld)
	:GameSystem(gameWorld, "draw")
{
}

void DrawSystem::draw()
{
	updateComponents();
	drawPhong();
	drawText();
}

void DrawSystem::drawPhong()
{
	Global::graphics.bindShader("phong");
	Global::graphics.setGlobalData(glm::vec3(0.15f, 1.0f, 1.0f));
	Global::graphics.setCameraData(getGameWorld()->getCamera());
	for (auto component : components) {
		component->drawPhong();
	}
}

void DrawSystem::drawText()
{
	Global::graphics.bindShader("text");
	std::shared_ptr<DrawComponent> drawComponent = nullptr;
	for (auto component : components) {
		component->drawText();
	}
}

bool DrawSystem::addComponent(std::shared_ptr<GameComponent> gameComponent)
{
	waitingList.push_back(gameComponent);
	return true;
}

void DrawSystem::updateComponents()
{
	components.insert(components.end(), waitingList.begin(), waitingList.end());
	waitingList.clear();
	for (int i = components.size() - 1; i >= 0; i--)
		if (!components[i]->isActive())
			components.erase(components.begin() + i);
}