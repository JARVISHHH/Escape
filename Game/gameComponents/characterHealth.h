#pragma once

#include <Game/components.h>
#include <Game/gameComponents/gameHandlerComponent.h>

class CharacterHealth : public HealthComponent {
public:
	CharacterHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject);

	void start() override;
	void update(double seconds) override;
	bool damage(int damageNumber) override;

private:
	std::shared_ptr<GameObject> gameHandlerObject;
	std::shared_ptr<GameHandlerComponent> gameHandler;

	std::shared_ptr<DrawComponent> drawComponent;
	float lastImmune = 0;
};