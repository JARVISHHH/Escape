#pragma once

#include <unordered_map>

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

class Screen
{
public:
    Screen();
    ~Screen();

    static std::unordered_map<int, bool> keyPressing;

    virtual void init();

    virtual void update(double seconds);
    virtual void draw();
    virtual void keyEvent(int key, int action);
    virtual void mousePosEvent(double xpos, double ypos);
    virtual void mouseButtonEvent(int button, int action);
    virtual void scrollEvent(double distance);
    virtual void windowResizeEvent(int width, int height);
    virtual void framebufferResizeEvent(int width, int height);

protected:
    std::shared_ptr<Camera> camera;
};