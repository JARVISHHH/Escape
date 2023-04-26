#include "gameSystem.h"

GameSystem::GameSystem(std::string tag) 
	: tag(tag)
{

}

GameSystem::GameSystem(std::shared_ptr<GameWorld> gameWorld, std::string tag)
	:gameWorld(gameWorld), tag(tag)
{
	gameWorld->addGameSystem(shared_from_this());
}

std::string GameSystem::getTag() const
{
	return tag;
}

void GameSystem::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
}