#pragma once

#include "uiElement.h"

class Text : public UIElement {
public:
	Text();

	void draw() override;
	void resize(glm::vec2 scale) override;

	void setWidth(float width);
	void setFontSize(float fontSize);
	void setContent(std::string content);
	void setFont(std::string font);
	void setColor(glm::vec3 color);

protected:
	std::string font = "opensans";
	float fontSize = 0.3f;
	std::string content = "";

	glm::vec3 color = glm::vec3(0, 0, 0);

	float width;

private:

};