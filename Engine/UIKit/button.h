#pragma once

#include "uiElement.h"

class Button : public UIElement {
public:
	Button(glm::vec3 localPosition, glm::vec3 size, void (*doAction)());

	void draw() override;
	void mouseButtonEvent(int button, int action) override;

private:
	bool inButton();
	void (*doAction)();
};