#pragma once

#include <unordered_map>

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

class GameWorld;

class Screen
{
public:
    static std::unordered_map<int, bool> keyPressing;
    static std::unordered_map<int, bool> mousePressing;

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
    std::shared_ptr<GameWorld> gameWorld;
};