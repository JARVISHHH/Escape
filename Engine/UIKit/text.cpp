#include "text.h"

void Text::draw()
{
	width = Global::graphics.getFramebufferSize().x;
	glm::vec2 anchorPosition = glm::vec2(position.x, position.y);
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont(font), content, anchorPosition, AnchorPoint::TopLeft, width, fontSize, 0.1f, color);
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