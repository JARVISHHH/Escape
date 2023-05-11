#pragma once

#include <Game/components.h>
#include <Engine/Game/map/map.h>

class LightComponent : public GameComponent {
public:
	LightComponent(std::shared_ptr<Map> map,
		std::string characterTag,
		std::unordered_map<std::shared_ptr<MapNode>, std::shared_ptr<Light>> totalLights);

	void start() override;
	void update(double seconds);

private:
	std::shared_ptr<Map> map;
	std::string characterTag;
	std::shared_ptr<GameObject> character;

	std::unordered_map<std::shared_ptr<MapNode>, std::shared_ptr<Light>> totalLights;
};
