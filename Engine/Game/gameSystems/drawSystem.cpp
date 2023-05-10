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
	// Shaodw
	Global::graphics.bindShader("shadow");
	auto& lights = Global::graphics.getLights();
	for (int i = 0; i < lights.size(); i++) {
		Global::graphics.setShadow(i);
		for (auto component : components) {
			component->drawPhong();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Phong
	Global::graphics.bindShader("phong");
	Debug::checkGLError();
	getGameWorld()->setFramebufferSize();
	Debug::checkGLError();
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 0.2f, 0.8f));
	Debug::checkGLError();
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Debug::checkGLError();
	// Set uniform
	Global::graphics.setGlobalData(glm::vec3(0.15f, 1.0f, 1.0f));
	Debug::checkGLError();
	Global::graphics.setCameraData(getGameWorld()->getCamera());
	Debug::checkGLError();
	// Set shadow maps
	Global::graphics.bindShadow();
	// Draw objects
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