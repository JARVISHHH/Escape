#include "gamehandler.h"
#include <Engine/Game/gameSystems/drawSystem.h>
#include <Engine/Game/gameSystems/tickSystem.h>

std::shared_ptr<GameObject> createGameHandler(std::shared_ptr<GameWorld> gameWorld, float maxTime)
{
	std::shared_ptr<GameObject> gameHandler = std::make_shared<GameObject>("gameHandler");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	std::shared_ptr<GameHandlerComponent> gameHandlerComponent = std::make_shared<GameHandlerComponent>(maxTime);

	// Add components to game objects
	gameHandler->addComponent(transformComponent);
	gameHandler->addComponent(gameHandlerComponent);

	// Add the gameObject to systems
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(gameHandlerComponent);
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(gameHandlerComponent);
	gameWorld->addGameObject(gameHandler);

	return gameHandler;
}