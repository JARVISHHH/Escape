#include "condition.h"

BTStatus Condition::update(double seconds)
{
	if (checkCondition(seconds)) return BT_SUCCESS;
	else return BT_FAIL;
}

void Condition::reset()
{

}