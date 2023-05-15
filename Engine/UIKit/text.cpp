#include "text.h"

void Text::draw()
{
	width = Global::graphics.getFramebufferSize().x;
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont(font), content, position, AnchorPoint::TopLeft, width, size, 0.1f, color);
}