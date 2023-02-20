#include "gameSystem.h"

GameSystem::GameSystem(std::string tag) 
	: tag(tag)
{

}

std::string GameSystem::getTag() const
{
	return tag;
}

bool GameSystem::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
	return true;
}
