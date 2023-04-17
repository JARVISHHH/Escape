#pragma once

#include <Engine/Game/gameComponent.h>

class BTNode;

class BehaviorComponent : public GameComponent, public std::enable_shared_from_this<BehaviorComponent> {
public:
	BehaviorComponent();
	BehaviorComponent(std::shared_ptr<BTNode> root);

	void start() override;
	void update(double seconds) override;

	void setRoot(std::shared_ptr<BTNode> root);

	std::unordered_map<std::string, std::shared_ptr<void>> blackBoard;

private:
	std::shared_ptr<BTNode> root = nullptr;
};