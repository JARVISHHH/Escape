#pragma once

#include "uiElement.h"

class Button : public UIElement {
public:
	void draw() override;
	void click() override;

protected:
	glm::vec3 size;

	virtual void action();

private:
	bool inButton();

};