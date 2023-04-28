#include "shootAction.h"

ShootAction::ShootAction()
{
}

void ShootAction::start()
{
	shootComponent = behaviorComponent->getGameObject()->getComponent<ShootComponent>("shoot");
}

void ShootAction::reset()
{
	if (status == BT_RUNNING) shootComponent->setStop(true);
	status = BT_FAIL;
}

BTStatus ShootAction::doAction(double seconds)
{
	shootComponent->setStop(false);
	status = BT_RUNNING;
	return status;
}