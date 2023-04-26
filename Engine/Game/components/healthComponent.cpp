#include "healthComponent.h"

HealthComponent::HealthComponent(int maxHealth)
	:GameComponent("health"), maxHealth(maxHealth), currentHealth(maxHealth)
{

}

void HealthComponent::update(double seconds)
{
	if(leftImmuneTime > 0) leftImmuneTime -= seconds;
}

bool HealthComponent::damage(int damageNumber)
{
	if (leftImmuneTime <= 0) {
		currentHealth = std::max(currentHealth - damageNumber, 0.0f);
		std::cout << "get damage, left health: " << currentHealth << std::endl;
		leftImmuneTime = immuneTime;
	}
	return true;
}

bool HealthComponent::heal(int healNumber)
{
	if (currentHealth == maxHealth) return false;
	currentHealth = std::min(currentHealth + healNumber, maxHealth);
	return true;
}