#pragma once

#include "Engine/Game/GameSystem.h"

class CameraSystem : public GameSystem {
public:
	CameraSystem(std::shared_ptr<Camera> camera);
	CameraSystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Camera> camera);

	void update(double seconds);
	void mousePosEvent(double xpos, double ypos);
	void scrollEvent(double distance);
	void windowResizeEvent(int width, int height);

	void setCamera(std::shared_ptr<Camera> camera);
	void setCharacter(std::shared_ptr<GameObject> character);

protected:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<GameObject> character;

	float rotateSpeed = 0.0025f;
	glm::vec2 previousMousePosition = { 0, 0 };
	glm::vec3 focus = {0, 0, 0};
	glm::vec3 focus2Camera = { 0, 0, 0 };

	glm::vec3 thirdPersonFoucsDiff = {0, 0.5, 0};

	bool firstPerson = true;
};
