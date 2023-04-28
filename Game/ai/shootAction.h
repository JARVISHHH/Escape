#pragma once

#include <Engine/Game/ai/behaviortree/action.h>
#include <Game/gameComponents/shootComponent.h>

class ShootAction : public Action {
public:
	ShootAction();

	void start() override;
	void reset() override;

protected:
	BTStatus doAction(double seconds);

private:
	std::shared_ptr<ShootComponent> shootComponent = nullptr;
};