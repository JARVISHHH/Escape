#include "action.h"

BTStatus Action::update(double seconds)
{
	return doAction(seconds);
}

void Action::reset()
{

}