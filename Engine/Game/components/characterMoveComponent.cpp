#include "characterMoveComponent.h"
#include <Engine/Game/components/transformComponent.h>

CharacterMoveComponent::CharacterMoveComponent()
	: GameComponent("characterMove")
{

}

void CharacterMoveComponent::update(double seconds)
{

	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (transformComponent == nullptr) {
		std::cerr << "CharatcerMoveComponent without a transformComponent." << std::endl;
		return;
	}

	auto camera = gameObject->getCamera();
	auto look = camera->getLook();

	glm::vec3 moveDir(0, 0, 0);

	if (Screen::keyPressing[moveKeys[0]]) {
		moveDir += glm::vec3(look.x, 0, look.z);
	}
	if (Screen::keyPressing[moveKeys[1]]) {
		moveDir += glm::vec3(-look.x, 0, -look.z);
	}
	if (Screen::keyPressing[moveKeys[2]]) {
		moveDir += glm::vec3(look.z, 0, -look.x);
	}
	if (Screen::keyPressing[moveKeys[3]]) {
		moveDir += glm::vec3(-look.z, 0, look.x);
	}

	if (moveDir != glm::vec3(0, 0, 0)) {
		moveDir = glm::normalize(moveDir);
		transformComponent->getModelTransform()->translate(speed * (float)seconds * moveDir);
		camera->translate(speed * (float)seconds * moveDir);
	}
}