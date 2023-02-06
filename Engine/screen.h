#pragma once

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

class Screen
{
public:
    Screen();
    ~Screen();
    void update(double seconds);
    void draw();
    void keyEvent(int key, int action);
    void mousePosEvent(double xpos, double ypos);
    void mouseButtonEvent(int button, int action);
    void scrollEvent(double distance);
    void windowResizeEvent(int width, int height);
    void framebufferResizeEvent(int width, int height);

private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<ModelTransform> modelTransform;

    bool keyStateW, keyStateA, keyStateS, keyStateD;
    bool mouseStateRight;
    glm::vec2 previousMousePosition;
};