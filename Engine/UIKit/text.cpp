#include "text.h"

Text::Text()
	:UIElement()
{
	width = Global::graphics.getFramebufferSize().x;
	renderType = AlphaRender;
}

void Text::draw()
{
	if (!show) return;
	width = Global::graphics.getFramebufferSize().x;
	glm::vec2 anchorPosition = glm::vec2(position.x, position.y);
	// Global::graphics.bindShader("ui");
	Global::graphics.drawUIText(Global::graphics.getFont(font), content, anchorPosition, AnchorPoint::TopLeft, width, fontSize, 0.1f, color);
}

void Text::resize(glm::vec2 scale)
{
	UIElement::resize(scale);
	width *= scale[0];
	fontSize *= scale[1];
}

void Text::setWidth(float width)
{
	this->width = width;
}

void Text::setFontSize(float fontSize)
{
	this->fontSize = fontSize;
}

void Text::setContent(std::string content)
{
	this->content = content;
}

void Text::setFont(std::string font)
{
	this->font = font;
}

void Text::setColor(glm::vec3 color)
{
	this->color = color;
}