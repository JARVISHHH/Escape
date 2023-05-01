#include "gameComponent.h"

GameComponent::GameComponent(std::string tag)
	: tag(tag)
{

}

GameComponent::~GameComponent()
{
	std::cout << "game component delete" << std::endl;
}

void GameComponent::start()
{
}

void GameComponent::update(double seconds)
{
}

void GameComponent::drawPhong()
{
}

void GameComponent::drawText()
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
	return gameObject.lock();
}