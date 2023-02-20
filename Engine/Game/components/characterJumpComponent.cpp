#include "Engine/screen.h"
#include "characterJumpComponent.h"
#include <Engine/Game/components/transformComponent.h>

CharacterJumpComponent::CharacterJumpComponent()
	: GameComponent("characterJump")
{

}

void CharacterJumpComponent::update(double seconds)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	if (transformComponent == nullptr) {
		std::cerr << "CharatcerJumpComponent without a transformComponent." << std::endl;
		return;
	}

	auto modelTransform = transformComponent->getModelTransform();
	auto bottomPosition = modelTransform->getPos().y - 0.5 * modelTransform->getScale().y;
	if (Screen::keyPressing[jumpKey] && bottomPosition == 0) {
		fallSpeed += jumpSpeed;
	}

	if (bottomPosition > 0) {
		fallSpeed -= gravity * seconds;
	}

	auto moveDistance = glm::vec3(0, fallSpeed * seconds, 0);
	if (moveDistance.y < 0 && bottomPosition < std::abs(moveDistance.y)) {
		fallSpeed = 0;
		moveDistance.y = -bottomPosition;
	}
	modelTransform->translate(glm::vec3(0, moveDistance.y, 0));
}