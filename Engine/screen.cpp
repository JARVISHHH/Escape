#include "screen.h"

Screen::Screen() :
	camera(std::make_shared<Camera>()),
	shape(Global::graphics.getShape("cube")), modelTransform(std::make_shared<ModelTransform>()),
	keyStateW(false), keyStateA(false), keyStateS(false), keyStateD(false),
	mouseStateRight(false) {

	modelTransform->rotate(glm::radians(30.0), glm::vec3(1, 0, 0));
	modelTransform->rotate(glm::radians(30.0), glm::vec3(0, 0, 1));
	modelTransform->translate(glm::vec3(0, 0, 3));

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
}

Screen::~Screen() {

}

void Screen::update(double t) {
	auto look = camera->getLook();

	// Move
	glm::vec3 moveDir(0, 0, 0);
	float speed = 0.1f;

	if (keyStateW) {
		moveDir += glm::vec3(look.x, 0, look.z);
	}
	if (keyStateS) {
		moveDir += glm::vec3(-look.x, 0, -look.z);
	}
	if (keyStateA) {
		moveDir += glm::vec3(look.z, 0, -look.x);
	}
	if (keyStateD) {
		moveDir += glm::vec3(-look.z, 0, look.x);
	}

	if (moveDir != glm::vec3(0, 0, 0)) {
		moveDir = glm::normalize(moveDir);
		camera->translate(speed * (float)t * moveDir);
	}
}

void Screen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 1.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong
	Global::graphics.bindShader("phong");
	Global::graphics.setGlobalData(glm::vec3(0.5f));
	Global::graphics.setCameraData(camera);
	Global::graphics.drawShape(shape, modelTransform, Global::graphics.getMaterial("grass"));

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Welcome to CS1950U!", glm::ivec2(0, 200), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.f, 0.1f, glm::vec3(1, 0, 1));
}

void Screen::keyEvent(int key, int action) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			keyStateW = true;
			break;
		case GLFW_KEY_A:
			keyStateA = true;
			break;
		case GLFW_KEY_S:
			keyStateS = true;
			break;
		case GLFW_KEY_D:
			keyStateD = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W:
			keyStateW = false;
			break;
		case GLFW_KEY_A:
			keyStateA = false;
			break;
		case GLFW_KEY_S:
			keyStateS = false;
			break;
		case GLFW_KEY_D:
			keyStateD = false;
			break;
		}
	}
}

void Screen::mousePosEvent(double xpos, double ypos) {
	auto look = camera->getLook();
	if (mouseStateRight) {
		camera->rotate(0.01f * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
		camera->rotate(0.01f * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
	}
	previousMousePosition = { xpos, ypos };
}

void Screen::mouseButtonEvent(int button, int action) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseStateRight = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseStateRight = false;
			break;
		}
	}
}

void Screen::scrollEvent(double distance) {

}

void Screen::framebufferResizeEvent(int width, int height) {
	Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void Screen::windowResizeEvent(int width, int height) {
	Global::graphics.setWindowSize(glm::ivec2(width, height));
	camera->resize(width, height);
}