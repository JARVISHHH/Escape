#include "gameComponent.h"

GameComponent::GameComponent(std::string tag)
	: tag(tag)
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