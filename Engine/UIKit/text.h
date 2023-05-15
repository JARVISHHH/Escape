#pragma once

#include "uiElement.h"

class Text : public UIElement {
public:
	void draw() override;

protected:
	float width;
	float fontSize = 0.3f;
	std::string content = "";
	std::string font = "opensans";

	glm::vec3 color(0, 0, 0);

private:

};