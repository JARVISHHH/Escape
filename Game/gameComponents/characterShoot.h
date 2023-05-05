#pragma once

#include <Game/components.h>

class CharacterShoot : public GameComponent {
public:
	CharacterShoot(std::shared_ptr<Camera> camera);

	void update(double seconds);

	std::string bulletTag = "bullet";

private:
	std::shared_ptr<Camera> camera;
	int shootKey = GLFW_MOUSE_BUTTON_LEFT;

	bool bulletLoaded = true;

	void shoot();
};