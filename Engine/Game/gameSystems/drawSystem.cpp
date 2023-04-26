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
	updateGameObjects();
	Global::graphics.bindShader("phong");
	Global::graphics.setGlobalData(glm::vec3(0.5f));
	Global::graphics.setCameraData(gameWorld->getCamera());
	std::shared_ptr<DrawComponent> drawComponent = nullptr;
	for (auto gameObject : gameObjects) {
		drawComponent = gameObject->getComponent<DrawComponent>("draw");
		if (drawComponent != nullptr) drawComponent->draw();
	}
}

bool DrawSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
	return true;
}

void DrawSystem::updateGameObjects()
{
	for (int i = gameObjects.size() - 1; i >= 0; i--)
		if (!gameObjects[i]->getActiveStatus())
			gameObjects.erase(gameObjects.begin() + i);
}