#include "characterShoot.h"

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
}