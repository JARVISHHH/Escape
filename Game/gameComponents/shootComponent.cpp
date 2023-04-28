#include "shootComponent.h"

ShootComponent::ShootComponent(std::string targetTag, bool stop)
	:GameComponent("shoot"), targetTag(targetTag), stop(stop)
{

}

void ShootComponent::start()
{
	auto targetObject = gameObject->getGameWorld()->getGameObject(targetTag);
	if (targetObject != nullptr) targetTransform = targetObject->getComponent<TransformComponent>("transform");
}

void ShootComponent::update(double seconds)
{
	if (stop) return;
	if (leftInterval < seconds) {
		shoot();
		leftInterval = shootInterval;
	}
	else {
		leftInterval -= seconds;
	}
}

void ShootComponent::setStop(bool stop)
{
	if (this->stop && !stop) leftInterval = 0;
	this->stop = stop;
}

void ShootComponent::shoot()
{
	std::cout << "shoot" << std::endl;
}