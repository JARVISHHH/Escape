#include "selector.h"

BTStatus Selector::update(double seconds)
{
	BTStatus finalStatus = BT_FAIL;

	for (int i = 0; i < children.size(); i++) {
		auto child = children[i];
		finalStatus = child->update(seconds);
		if (finalStatus != BT_FAIL) {
			if (i != lastRunningIndex && lastRunning != nullptr)
				lastRunning->reset();
			setLastRunning(i);
			break;
		}
		child->reset();
	}

	return finalStatus;
}

void Selector::reset()
{
	lastRunning->reset();;
	setLastRunning(0);
}