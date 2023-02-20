#include "Engine/Game/gameObject.h"
#include "drawSystem.h"
#include "Engine/Game/components/drawComponent.h"

void DrawSystem::draw()
{
	std::shared_ptr<DrawComponent> drawComponent = nullptr;
	for (auto gameObject : gameObjects) {
		drawComponent = gameObject->getComponent<DrawComponent>("draw");
		if (drawComponent != nullptr) drawComponent->draw();
	}
}