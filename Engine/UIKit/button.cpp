#include "button.h"

void Button::draw()
{
	UIElemnet::draw();
}

void Button::click()
{
	if (inButton()) action();
} 

void Button::action()
{
}

bool Button::inButton()
{
	if (Screen::mousePos[0] < position[0]) return false;
	if (Screen::mousePos[0] > position[0] + size[0]) return false;
	if (Screen::mousePos[1] < position[1]) return false;
	if (Screen::mousePos[1] > position[1] + size[1]) return false;
	return true;
}