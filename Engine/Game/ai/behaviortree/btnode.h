#pragma once

#include <Engine/Game/components/behaviorComponent.h>

enum BTStatus {
	BT_SUCCESS,
	BT_FAIL, 
	BT_RUNNING
};

class BTNode {
public:
	virtual void start();
	virtual BTStatus update(double seconds) = 0;
	virtual void reset() = 0;

	virtual void setBehaviorComponent(std::shared_ptr<BehaviorComponent>) { this->behaviorComponent = behaviorComponent; }

protected:
	std::shared_ptr<BehaviorComponent> behaviorComponent = nullptr;
};