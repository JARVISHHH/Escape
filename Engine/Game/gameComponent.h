#pragma once

#include "Engine/Game/gameObject.h"

class GameComponent
{
public:
	GameComponent(std::string tag);

	std::string getTag() const;
	void setGameObject(std::shared_ptr<GameObject> gameObject);
	std::shared_ptr<GameObject> getGameObject();

protected:
	std::shared_ptr<GameObject> gameObject;
	const std::string tag;
};