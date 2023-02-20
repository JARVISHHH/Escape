#pragma once

#include "Engine/screen.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include "Engine/Game/gameWorld.h"

class GameScreen : public Screen {

public:
	GameScreen();
	~GameScreen();

    void init();

    void update(double seconds) override;
    void draw() override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;

private:
    std::shared_ptr<GameWorld> gameWorld;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<ModelTransform>> modelTransforms;

    std::shared_ptr<Shape> character;
    std::shared_ptr<ModelTransform> characterModelTransform;

    float gravity = 10;
    float jumpSpeed = 5;
    float characterFallSpeed;

    bool firstPerson = true;

    bool keyStateW, keyStateA, keyStateS, keyStateD, keyStateSpace;
    bool mouseStateRight;
    glm::vec2 previousMousePosition = { 0, 0 };
};