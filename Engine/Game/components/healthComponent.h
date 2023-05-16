#pragma once

#include <Engine/Game/gameComponent.h>

class HealthComponent : public GameComponent {
public:
	HealthComponent(int maxHealth = 10);

	void update(double seconds) override;

	virtual bool damage(int damageNumber);
	virtual bool heal(int healNumber);

	float getCurrentHealth() { return currentHealth; }

protected:
	float maxHealth;
	float currentHealth;

	float immuneTime = 2;
	float leftImmuneTime = 0;
};