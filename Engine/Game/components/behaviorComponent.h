#pragma once

#include <Engine/Game/gameComponent.h>

class BTNode;

class BehaviorComponent : public GameComponent {
public:
	BehaviorComponent();
	BehaviorComponent(std::shared_ptr<BTNode> root);

	void start() override;
	void update(double seconds) override;

	void setRoot(std::shared_ptr<BTNode> root);

private:
	std::shared_ptr<BTNode> root = nullptr;
};