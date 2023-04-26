#pragma once

#include <Engine/Game/gameComponent.h>

class HealthComponent : public GameComponent {
public:
	HealthComponent(int maxHealth = 10);

	void update(double seconds) override;

	bool damage(int damageNumber);
	bool heal(int healNumber);

	float getCurrentHealth() { return currentHealth; }

private:
	float maxHealth;
	float currentHealth;

	float immuneTime = 2;
	float leftImmuneTime = 0;
};