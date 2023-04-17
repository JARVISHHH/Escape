#include "sequence.h"

BTStatus Sequence::update(double seconds)
{
	BTStatus finalStatus = BT_SUCCESS;

	for (int i = lastRunningIndex; i < children.size(); i++) {
		auto child = children[i];
		finalStatus = child->update(seconds);
		if (finalStatus != BT_SUCCESS) {
			setLastRunning(i);
			break;
		}
	}

	if (finalStatus == BT_SUCCESS) reset();

	return finalStatus;
}

void Sequence::reset()
{
	for (int i = 0; i < lastRunningIndex; i++)
		children[i]->reset();
	if (lastRunning != nullptr) lastRunning->reset();
	setLastRunning(0);
}