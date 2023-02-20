#include "Engine/Game/gameObject.h"
#include "drawSystem.h"
#include "Engine/Game/components/drawComponent.h"

DrawSystem::DrawSystem() : GameSystem("draw")
{
	
}

void DrawSystem::draw()
{
	Global::graphics.bindShader("phong");
	Global::graphics.setGlobalData(glm::vec3(0.5f));
	Global::graphics.setCameraData(gameWorld->getCamera());
	std::shared_ptr<DrawComponent> drawComponent = nullptr;
	for (auto gameObject : gameObjects) {
		drawComponent = gameObject->getComponent<DrawComponent>("draw");
		if (drawComponent != nullptr) drawComponent->draw();
	}
}