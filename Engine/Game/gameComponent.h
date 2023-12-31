#pragma once

#include "Engine/Game/gameObject.h"

class GameComponent
{
public:
	GameComponent(std::string tag);
	~GameComponent();

	virtual void start();
	virtual void update(double seconds);
	virtual void drawPhong();
	virtual void drawText();

	std::string getTag() const;
	void setGameObject(std::shared_ptr<GameObject> gameObject);
	std::shared_ptr<GameObject> getGameObject();
	bool isActive();

protected:
	std::weak_ptr<GameObject> gameObject;
	const std::string tag;
};