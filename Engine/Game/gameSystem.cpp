#include "gameSystem.h"

GameSystem::GameSystem(std::string tag) 
	: tag(tag)
{

}

GameSystem::GameSystem(std::shared_ptr<GameWorld> gameWorld, std::string tag)
	: gameWorld(gameWorld), tag(tag)
{
	gameWorld->addGameSystem(shared_from_this());
}

GameSystem::~GameSystem()
{
	//std::cout << "game system delete" << std::endl;
}

void GameSystem::start()
{
}

std::string GameSystem::getTag() const
{
	return tag;
}

std::shared_ptr<GameWorld> GameSystem::getGameWorld()
{
	return gameWorld.lock();
}

void GameSystem::setGameWorld(std::shared_ptr<GameWorld> gameWorld)
{
	this->gameWorld = gameWorld;
}