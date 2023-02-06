#include "gameScreen.h"

extern std::shared_ptr<App> app;

GameScreen::GameScreen() : 
	Screen(),
	characterFallSpeed(0),
	keyStateW(false), keyStateA(false), keyStateS(false), keyStateD(false), keyStateSpace(false),
	mouseStateRight(false) {

	int xNum = 8, zNum = 8;

	for (int x = 0; x < xNum; x++) {
		for (int z = 0; z < zNum; z++) {
			shapes.push_back(Global::graphics.getShape("quad"));
			modelTransforms.push_back(std::make_shared<ModelTransform>());
			modelTransforms[x * xNum + z]->translate(glm::vec3(x - xNum / 2, -0.125, z - zNum / 2));
		}
	}

	character = Global::graphics.getShape("cube");
	characterModelTransform = std::make_shared<ModelTransform>();
	characterModelTransform->scale(0.25);

	camera->setPos(characterModelTransform->getPos());

	Global::graphics.addMaterial("grass", "Resources/Images/grass.png");
}

GameScreen::~GameScreen() {

}

void GameScreen::update(double seconds) {
	auto look = camera->getLook();

	// Move
	glm::vec3 moveDir(0, 0, 0);
	float speed = 1.5f;

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
		characterModelTransform->translate(speed * (float)seconds * moveDir);
		camera->translate(speed * (float)seconds * moveDir);
	}

	auto characterPosition = characterModelTransform->getPos();

	// Jump
	if (keyStateSpace && characterPosition.y == 0) {
		characterFallSpeed += jumpSpeed;
		//std::cout << "jump" << std::endl;
	}

	// Fall
	if (characterPosition.y > 0) {
		characterFallSpeed -= gravity * seconds;
	}
	
	auto moveDistance = glm::vec3(0, characterFallSpeed * seconds, 0);
	if (moveDistance.y < 0 && characterPosition.y < std::abs(moveDistance.y)) {
		characterFallSpeed = 0;
		moveDistance.y = -characterPosition.y;
	}
	characterModelTransform->translate(glm::vec3(0, moveDistance.y, 0));
	camera->translate(glm::vec3(0, moveDistance.y, 0));

	//std::cout << "time: " << seconds << ". speed " << characterFallSpeed << ". position.y " << characterModelTransform->getPos().y << std::endl;
}

void GameScreen::draw() {
	// Set background color
	Global::graphics.setClearColor(glm::vec3(0.0f, 0.0f, 1.0f));
	Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong
	Global::graphics.bindShader("phong");
	Global::graphics.setGlobalData(glm::vec3(0.5f));
	Global::graphics.setCameraData(camera);
	for(int i = 0; i < shapes.size(); i++) Global::graphics.drawShape(shapes[i], modelTransforms[i], Global::graphics.getMaterial("grass"));
	Global::graphics.drawShape(character, characterModelTransform, nullptr);

	// Text
	Global::graphics.bindShader("text");
	Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Press B to go back.", glm::ivec2(0, 200), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.f, 0.1f, glm::vec3(1, 0, 1));
}

void GameScreen::keyEvent(int key, int action) {
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
		case GLFW_KEY_B:
			app->activateScreen("menu");
			break;
		case GLFW_KEY_SPACE:
			keyStateSpace = true;
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
		case GLFW_KEY_SPACE:
			keyStateSpace = false;
			break;
		}
	}
}

void GameScreen::mousePosEvent(double xpos, double ypos) {
	float rotateSpeed = 0.005f;
	
	auto look = camera->getLook();
	if (mouseStateRight) {
		if (firstPerson) {
			camera->rotate(rotateSpeed * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
		}
		else {
			camera->rotate(rotateSpeed * (xpos - previousMousePosition.x), glm::vec3(0, 1, 0));
			camera->rotate(rotateSpeed * (ypos - previousMousePosition.y), glm::vec3(look.z, 0, look.x));
			auto characterPosition = characterModelTransform->getPos(), cameraPosition = camera->getPos();
			camera->setPos(characterPosition - camera->getLook() * (glm::length(characterPosition - cameraPosition)));
		}
	}
	previousMousePosition = { xpos, ypos };
}

void GameScreen::mouseButtonEvent(int button, int action) {
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

void GameScreen::scrollEvent(double distance) {
	auto oldPosition = camera->getPos(), look = camera->getLook(), characterPosition = characterModelTransform->getPos();
	auto newPosition = oldPosition + look * (float)distance;
	if (firstPerson) {
		if (distance >= 0) newPosition = characterPosition;
		else firstPerson = false;
	}
	else {
		if (glm::dot(characterPosition - oldPosition, characterPosition - newPosition) <= 0) {
			newPosition = characterPosition;
			firstPerson = true;
		}
	}
	camera->setPos(newPosition);
}

void GameScreen::framebufferResizeEvent(int width, int height) {
	Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void GameScreen::windowResizeEvent(int width, int height) {
	Global::graphics.setWindowSize(glm::ivec2(width, height));
	camera->resize(width, height);
}