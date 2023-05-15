#include "button.h"

Button::Button(glm::vec3 localPosition, glm::vec3 size, void (*doAction)())
	:UIElement(), size(size), doAction(doAction)
{
	setLocalPosition(localPosition);
}

void Button::draw()
{
	UIElement::draw();
}

void Button::mouseButtonEvent(int button, int action)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
		std::cout << "button tried" << std::endl;
		if (!inButton()) return;
		std::cout << "do action" << std::endl;
		doAction();
	}
}

bool Button::inButton()
{
	int height = Global::graphics.getFramebufferSize().y;
	std::cout << "screen " << Screen::mousePos[0] << " " << height - Screen::mousePos[1] << std::endl;
	std::cout << "pos " << position[0] << " " << position[1] << std::endl;
	std::cout << "size " << size[0] << " " << size[1] << std::endl;
	if (Screen::mousePos[0] < position[0]) return false;
	if (Screen::mousePos[0] > position[0] + size[0]) return false;
	if (height - Screen::mousePos[1] > position[1]) return false;
	if (height - Screen::mousePos[1] < position[1] - size[1]) return false;
	return true;
}