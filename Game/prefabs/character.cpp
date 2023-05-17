#include "character.h"
#include <Game/gameComponents/characterMoving.h>

std::shared_ptr<GameObject> createCharacter(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, std::shared_ptr<GameObject> gameHandlerObject)
{
	std::shared_ptr<GameObject> character = std::make_shared<GameObject>("character");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.1);
	modelTransform->translate(pos);
	gameWorld->getCamera()->setPos(modelTransform->getPos());
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("character", "trashcan", "trashcanNormal");
	// Physics component
	std::shared_ptr<PhysicsComponent> physicsComponent = std::make_shared<PhysicsComponent>();
	// CharacterMoveComponent
	std::shared_ptr<CharacterMoveComponent> characterMoveComponent = std::make_shared<CharacterMoveComponent>();
	// CharacterJumpComponent
	std::shared_ptr<CharacterJumpComponent> characterJumpComponent = std::make_shared<CharacterJumpComponent>();
	// Collision component
	modelTransform = std::make_shared<ModelTransform>();
	modelTransform->scale(3.3);
	//modelTransform->translate(glm::vec3(0, 1.7, 0));
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>(modelTransform);
	// Collision response component
	std::shared_ptr<CharacterCollisionResponse> collisionResponseComponent = std::make_shared<CharacterCollisionResponse>(gameHandlerObject);
	// Health component
	std::shared_ptr<CharacterHealth> healthComponent = std::make_shared<CharacterHealth>(10, gameHandlerObject);
	// Shootng component
	std::shared_ptr<CharacterShoot> characterShoot = std::make_shared<CharacterShoot>(gameWorld->getCamera());

	// Add components to game objects
	character->addComponent(transformComponent);
	character->addComponent(drawComponent);
	character->addComponent(physicsComponent);
	character->addComponent(characterMoveComponent);
	character->addComponent(characterJumpComponent);
	character->addComponent(cylinderComponent);
	character->addComponent(collisionResponseComponent);
	character->addComponent(healthComponent);
	character->addComponent(characterShoot);

	gameWorld->getGameSystem<CameraSystem>("camera")->setCharacter(character);
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<PhysicsSystem>("physics")->addGameObject(character);
	gameWorld->getGameSystem<CharacterControllerSystem>("characterController")->setCharatcer(character);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(character, "character");
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(healthComponent);
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(characterShoot);
	gameWorld->addGameObject(character);

	return character;
}

std::shared_ptr<GameObject> createMovingCharacter(std::shared_ptr<GameWorld> gameWorld, glm::vec3 pos, std::shared_ptr<Map> map, std::shared_ptr<MapNode> mapNode)
{
	std::shared_ptr<GameObject> character = std::make_shared<GameObject>("character");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	auto modelTransform = transformComponent->getModelTransform();
	modelTransform->scale(0.1);
	modelTransform->translate(pos);
	gameWorld->getCamera()->setPos(modelTransform->getPos());
	// Draw component
	std::shared_ptr<DrawComponent> drawComponent = std::make_shared<DrawComponent>("character", "trashcan", "trashcanNormal");
	// Collision component
	modelTransform = std::make_shared<ModelTransform>();
	modelTransform->scale(3.3);
	//modelTransform->translate(glm::vec3(0, 1.7, 0));
	std::shared_ptr<CylinderComponent> cylinderComponent = std::make_shared<CylinderComponent>(modelTransform);
	// Moving compinent
	std::shared_ptr<CharacterMoving> characterMoving = std::make_shared<CharacterMoving>(map, mapNode);


	// Add components to game objects
	character->addComponent(transformComponent);
	character->addComponent(drawComponent);
	character->addComponent(cylinderComponent);
	character->addComponent(characterMoving);

	gameWorld->getGameSystem<CameraSystem>("camera")->setCharacter(character);
	gameWorld->getGameSystem<DrawSystem>("draw")->addComponent(drawComponent);
	gameWorld->getGameSystem<CollisionSystem>("collision")->addGameObject(character, "character");
	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(characterMoving);
	gameWorld->addGameObject(character);

	return character;
}