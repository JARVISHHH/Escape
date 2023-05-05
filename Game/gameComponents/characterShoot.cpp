#include "characterShoot.h"
#include <Game/prefabs/bullet.h>

CharacterShoot::CharacterShoot(std::shared_ptr<Camera> camera)
	:GameComponent("characterShoot"), camera(camera)
{
}

void CharacterShoot::update(double seconds)
{
	if (Screen::mousePressing[shootKey]) {
		if (bulletLoaded) {
			shoot();
			bulletLoaded = false;
		}
	}
	else {
		bulletLoaded = true;
	}
}

void CharacterShoot::shoot()
{
	auto direction = camera->getLook();
	createBullet(getGameObject()->getGameWorld(), "cylinder", "", camera->getPos(), glm::normalize(direction), bulletTag);
}