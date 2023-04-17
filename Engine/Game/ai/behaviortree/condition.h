#pragma once

#include "btnode.h"

class Condition : public BTNode {
public:
	BTStatus update(double seconds) override;
	void reset() override;

protected:
	virtual bool checkCondition(double seconds) = 0;
};
