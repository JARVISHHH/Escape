#include "lights.h"

std::shared_ptr<GameObject> createLights(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<Map> map) {
	if (map == nullptr) return nullptr;

	std::shared_ptr<GameObject> lights = std::make_shared<GameObject>("lights");

	// Create components
	// Transform Component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	// Light(draw) component
	std::unordered_map<std::shared_ptr<MapNode>, std::shared_ptr<Light>> totalLights;
	auto& mapNodes = map->leaves;
	for (auto mapNode : mapNodes) {
		auto room = mapNode->room;
		glm::vec3 pos = glm::vec3(room->getCenter()[0], room->getMinPoint()[1] + HEIGHT - 1, room->getCenter()[2]);
		auto light = std::make_shared<Light>(LightType::POINT, pos, glm::vec3(5.0f));
		totalLights[mapNode] = light;
	}
	std::shared_ptr<LightComponent> lightComponent = std::make_shared<LightComponent>(map, "character", totalLights);

	// Add components to game objects
	lights->addComponent(transformComponent);
	lights->addComponent(lightComponent);

	gameWorld->getGameSystem<TickSystem>("tick")->addComponent(lightComponent);
	gameWorld->addGameObject(lights);

	return lights;
}