#include "cameraSystem.h"
#include <Engine/Game/components/transformComponent.h>

CameraSystem::CameraSystem(std::shared_ptr<Camera> camera)
	: GameSystem("camera")
{
	this->camera = camera;
}

CameraSystem::CameraSystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Camera> camera)
	: GameSystem(gameWorld, "camera")
{
	this->camera = camera;
}

void CameraSystem::update(double seconds)
{
	auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();
	auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
	if (firstPerson) focus = characterPosition;
	else focus = characterPosition + thirdPersonFoucsDiff;
	camera->setPos(focus + focus2Camera);
}

void CameraSystem::mousePosEvent(double xpos, double ypos)
{
	auto look = camera->getLook();
	if (Screen::mousePressing[GLFW_MOUSE_BUTTON_RIGHT]) {
		if (firstPerson) {
			camera->rotate(rotateSpeed * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
			//camera->rotate(rotateSpeed * (previousMousePosition.x - xpos), glm::vec3(0, 1, 0));
			//camera->rotate(rotateSpeed * (previousMousePosition.y - ypos), glm::vec3(look.z, 0, look.x));
		}
		else {
			camera->rotate(rotateSpeed * (previousMousePosition.x - xpos), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (previousMousePosition.y - ypos), glm::vec3(look.z, 0, look.x));

			auto characterModelTransform = character->getComponent<TransformComponent>("transform")->getModelTransform();
			auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
			focus = characterPosition + thirdPersonFoucsDiff;
			focus2Camera = -camera->getLook() * (glm::length(focus - cameraPosition));
		}
	}
	previousMousePosition = { xpos, ypos };
}

void CameraSystem::scrollEvent(double distance)
{
	auto oldFocus2Camera = focus2Camera, look = camera->getLook();
	focus2Camera += look * (float)distance;
	if (firstPerson) {
		if (distance >= 0) focus2Camera = glm::vec3(0, 0, 0);
		else firstPerson = false;
	}
	else {
		if (glm::dot(oldFocus2Camera, focus2Camera) <= 0) {
			focus2Camera = glm::vec3(0, 0, 0);
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