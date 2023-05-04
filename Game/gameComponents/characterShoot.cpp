#include "characterShoot.h"
#include <Game/prefabs/bullet.h>

CharacterShoot::CharacterShoot()
	:GameComponent("characterShoot")
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
	createBullet(getGameObject()->getGameWorld(), "cylinder", "", camera->getPos(), glm::normalize(direction));
}