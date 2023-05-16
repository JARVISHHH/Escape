#pragma once

#include <Game/components.h>
#include <Game/gameComponents/gameHandlerComponent.h>
#include <Engine/UIKit/uiElement.h>

class CharacterHealth : public HealthComponent {
public:
	CharacterHealth(int maxHealth, std::shared_ptr<GameObject> gameHandlerObject);

	void start() override;
	void update(double seconds) override;
	bool damage(int damageNumber) override;
	bool heal(int healNumber) override;
	void setHP(std::shared_ptr<UIElement> hp);

private:
	std::shared_ptr<GameObject> gameHandlerObject;
	std::shared_ptr<GameHandlerComponent> gameHandler;

	std::shared_ptr<DrawComponent> drawComponent;
	std::shared_ptr<UIElement> hp;
	float lastImmune = 0;
};