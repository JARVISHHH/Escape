#include "cameraSystem.h"
#include <Engine/Game/components/transformComponent.h>

CameraSystem::CameraSystem(std::shared_ptr<Camera> camera, std::shared_ptr<GameObject> character)
	: GameSystem("camera")
{
	this->camera = camera;
	this->character = character;
}

void CameraSystem::update(double seconds)
{
	auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();
	auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
	camera->setPos(characterPosition + vector2Camera);
}

void CameraSystem::mousePosEvent(double xpos, double ypos)
{
	auto look = camera->getLook();
	if (Screen::mousePressing[GLFW_MOUSE_BUTTON_RIGHT]) {
		if (firstPerson) {
			camera->rotate(rotateSpeed * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
		}
		else {
			camera->rotate(rotateSpeed * (previousMousePosition.x - xpos), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (previousMousePosition.y - ypos), glm::vec3(look.z, 0, look.x));

			auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();
			auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
			vector2Camera = -camera->getLook() * (glm::length(characterPosition - cameraPosition));
		}
	}
	previousMousePosition = { xpos, ypos };
}

void CameraSystem::scrollEvent(double distance)
{
	auto oldVector2Camera = vector2Camera, look = camera->getLook();
	vector2Camera += look * (float)distance;
	if (firstPerson) {
		if (distance >= 0) vector2Camera = glm::vec3(0, 0, 0);
		else firstPerson = false;
	}
	else {
		if (glm::dot(oldVector2Camera, vector2Camera) <= 0) {
			vector2Camera = glm::vec3(0, 0, 0);
			firstPerson = true;
		}
	}
}

void CameraSystem::windowResizeEvent(int width, int height)
{
	camera->resize(width, height);
}

void CameraSystem::setCamera(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void CameraSystem::setCharacter(std::shared_ptr<GameObject> character)
{
	this->character = character;
}