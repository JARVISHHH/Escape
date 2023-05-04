#pragma once

#include <Game/components.h>

class CharacterShoot : public GameComponent {
public:
	CharacterShoot();

	void update(double seconds);

private:
	std::shared_ptr<Camera> camera;
	int shootKey = GLFW_MOUSE_BUTTON_LEFT;

	bool bulletLoaded = true;

	void shoot();
};