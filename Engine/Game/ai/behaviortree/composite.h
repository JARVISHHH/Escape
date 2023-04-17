#pragma once

#include <vector>
#include <memory>

#include "btnode.h"

class Composite : public BTNode {
public:
	void start() override;
	virtual BTStatus update(double seconds) = 0;
	virtual void reset() = 0;

	void addChild(std::shared_ptr<BTNode> child);
	void setBehaviorComponent(std::shared_ptr<BehaviorComponent> behaviorComponent);

protected:
	void setLastRunning(int index) {
		lastRunningIndex = index;
		if (index < 0 || index >= children.size()) lastRunning = nullptr;
		else lastRunning = children[index];
	}

	std::vector<std::shared_ptr<BTNode>> children;
	std::shared_ptr<BTNode> lastRunning = nullptr;
	int lastRunningIndex = 0;
};