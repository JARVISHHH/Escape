#pragma once

#include "Engine/screen.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

class GameScreen : public Screen {

public:
	GameScreen();
	~GameScreen();

    void update(double seconds) override;
    void draw() override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
    void windowResizeEvent(int width, int height) override;
    void framebufferResizeEvent(int width, int height) override;

private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<ModelTransform> modelTransform;

    bool keyStateW, keyStateA, keyStateS, keyStateD;
    bool mouseStateRight;
    glm::vec2 previousMousePosition = { 0, 0 };
};