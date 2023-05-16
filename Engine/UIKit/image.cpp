#include "image.h"

void Image::draw()
{
	UIElement::draw();
	// Global::graphics.bindShader("ui");
	Global::graphics.drawUI(shared_from_this());
}