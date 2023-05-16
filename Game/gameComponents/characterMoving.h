#pragma once

#include <Game/components.h>
#include <Engine/Game/map/map.h>
#include <Engine/Game/map/mapnode.h>

class CharacterMoving : public GameComponent {
public:
	CharacterMoving(std::shared_ptr<Map> map, std::shared_ptr<MapNode> mapNode);

	void start() override;
	void update(double seconds) override;

private:
	std::shared_ptr<Map> map;
	std::shared_ptr<MapNode> mapNode;

	float speed = 3;
	std::shared_ptr<TransformComponent> transformComponent;
	glm::vec3 nextPos;

	void findNextPos();
};