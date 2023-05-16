#include "characterMoving.h"

CharacterMoving::CharacterMoving(std::shared_ptr<Map> map, std::shared_ptr<MapNode> mapNode)
	:GameComponent("characterMoving"), map(map), mapNode(mapNode)
{
}

void CharacterMoving::start()
{
	transformComponent = getGameObject()->getComponent<TransformComponent>("transform");
	findNextPos();
}

void CharacterMoving::update(double seconds)
{
	auto direction = nextPos - transformComponent->getModelTransform()->getPos();
	auto distance = glm::length(direction);
	while (distance < seconds * speed) {
		seconds -= distance / speed;
		transformComponent->getModelTransform()->setPos(nextPos);
		findNextPos();
		direction = nextPos - transformComponent->getModelTransform()->getPos();
		distance = glm::length(direction);
	}
	transformComponent->getModelTransform()->translate(glm::normalize(direction) * speed * (float)seconds);
}

void CharacterMoving::findNextPos()
{
	auto connectors = map->findConnectors(mapNode);
	int size = connectors.size();
	mapNode = connectors[rand() % size];
	auto roomCenter = mapNode->room->getCenter();
	nextPos = glm::vec3(roomCenter[0], mapNode->room->getMinPoint()[1] + 2, roomCenter[2]) + glm::vec3(0, 1, 0);
}