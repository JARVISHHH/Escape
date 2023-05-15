#pragma once

#include "uiElement.h"

class Label : public UIElement {
public:
	void draw() override;

protected:
	glm::vec2 size;

private:

};