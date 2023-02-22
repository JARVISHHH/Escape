#include "Engine/screen.h"
#include "characterJumpComponent.h"
#include <Engine/Game/components/transformComponent.h>
#include <Engine/Game/components/physicsComponent.h>

CharacterJumpComponent::CharacterJumpComponent()
	: GameComponent("characterJump")
{

}

void CharacterJumpComponent::update(double seconds)
{
	auto transformComponent = gameObject->getComponent<TransformComponent>("transform");
	auto physicsComponent = gameObject->getComponent<PhysicsComponent>("physics");
	if (transformComponent == nullptr || physicsComponent == nullptr) {
		std::cerr << "CharatcerJumpComponent with incomplete components." << std::endl;
		return;
	}

	auto modelTransform = transformComponent->getModelTransform();
	auto bottomPosition = modelTransform->getPos().y - 0.5 * modelTransform->getScale().y;
	if (Screen::keyPressing[jumpKey] && bottomPosition == 0) {
		physicsComponent->applyVelocity(glm::vec3(0, jumpSpeed, 0));
	}
}