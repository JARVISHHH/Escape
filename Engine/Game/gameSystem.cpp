#include "gameSystem.h"

GameSystem::GameSystem(std::string tag) 
	: tag(tag)
{

}

std::string GameSystem::getTag() const
{
	return tag;
}

void GameSystem::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
}