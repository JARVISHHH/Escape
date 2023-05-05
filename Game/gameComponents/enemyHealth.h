#pragma once

#include <Game/components.h>
#include <Game/gameComponents/gameHandlerComponent.h>

class EnemyHealth : public HealthComponent {
public:
	EnemyHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject);

	void start() override;
	bool damage(int damageNumber) override;

private:
private:
	std::shared_ptr<GameObject> gameHandlerObject;
	std::shared_ptr<GameHandlerComponent> gameHandler;
};