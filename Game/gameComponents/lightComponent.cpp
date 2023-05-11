#include "lightComponent.h"

LightComponent::LightComponent(std::shared_ptr<Map> map,
	std::string characterTag, 
	std::unordered_map<std::shared_ptr<MapNode>, std::shared_ptr<Light>> totalLights)
	:GameComponent("light"), map(map), characterTag(characterTag), totalLights(totalLights)
{
}

void LightComponent::start()
{
	character = getGameObject()->getGameWorld()->getGameObject(characterTag);
}

void LightComponent::update(double seconds)
{
	auto mapNode = map->findNode(character);
	if (mapNode == nullptr) return;
	std::vector<std::shared_ptr<Light>> lights;
	lights.push_back(totalLights[mapNode]);
	for (auto& connectors : map->getConnectors()) {
		if (connectors.first == mapNode) lights.push_back(totalLights[connectors.second]);
		if (connectors.second == mapNode) lights.push_back(totalLights[connectors.first]);
	}
	Global::graphics.bindShader("phong");
	Global::graphics.setLights(lights);
}