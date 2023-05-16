#include "image.h"

void Image::draw()
{
	if (!show) return;
	UIElement::draw();
	Global::graphics.drawUI(shared_from_this());
}