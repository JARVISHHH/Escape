#pragma once

#include "btnode.h"

class Action : public BTNode {
public:
	BTStatus update(double seconds) override;
	void reset() override;

protected:
	virtual BTStatus doAction(double seconds) = 0;

	BTStatus status = BT_FAIL;

private:


};