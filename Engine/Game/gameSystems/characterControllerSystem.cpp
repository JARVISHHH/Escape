#include "characterControllerSystem.h"
#include <Engine/Game/components/characterMoveComponent.h>
#include <Engine/Game/components/characterJumpComponent.h>

CharacterControllerSystem::CharacterControllerSystem() 
	: GameSystem("characterController")
{

}

bool CharacterControllerSystem::setCharatcer(std::shared_ptr<GameObject> character)
{
	this->character = character;
	return true;
}

void CharacterControllerSystem::update(double seconds)
{
	auto characterMoveComponent = character->getComponent<CharacterMoveComponent>("characterMove");
	if (characterMoveComponent != nullptr) characterMoveComponent->update(seconds);

	auto characterJumpComponent = character->getComponent<CharacterJumpComponent>("characterJump");
	if (characterJumpComponent != nullptr) characterJumpComponent->update(seconds);
}