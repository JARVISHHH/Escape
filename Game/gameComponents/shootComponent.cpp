#include "shootComponent.h"

#include <Game/prefabs/projectile.h>

ShootComponent::ShootComponent(std::string targetTag, bool stop)
	:GameComponent("shoot"), targetTag(targetTag), stop(stop)
{

}

void ShootComponent::start()
{
	transform = getGameObject()->getComponent<TransformComponent>("transform");

	auto targetObject = getGameObject()->getGameWorld()->getGameObject(targetTag);
	if (targetObject != nullptr) targetTransform = targetObject->getComponent<TransformComponent>("transform");

	stop = false;
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
	auto direction = targetTransform->getModelTransform()->getPos() - transform->getModelTransform()->getPos();
	createProjectile(getGameObject()->getGameWorld(), "cylinder", "monomi", transform->getModelTransform()->getPos(), glm::normalize(direction));
}