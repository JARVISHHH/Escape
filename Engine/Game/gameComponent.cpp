#include "gameComponent.h"

GameComponent::GameComponent(std::string tag)
	: tag(tag)
{

}

void GameComponent::start()
{
}

void GameComponent::update(double seconds)
{
}

std::string GameComponent::getTag() const
{
	return tag;
}

void GameComponent::setGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObject = gameObject;
}

std::shared_ptr<GameObject> GameComponent::getGameObject()
{
	return gameObject;
}