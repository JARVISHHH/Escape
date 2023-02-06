#pragma once

#include <unordered_map>

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include "screen.h"

class Application
{
public:
    Application();
    ~Application();

    void addScreen(std::string name, std::shared_ptr<Screen> screen);
    void activateScreen(std::string name);

    void update(double seconds);
    void draw();
    void keyEvent(int key, int action);
    void mousePosEvent(double xpos, double ypos);
    void mouseButtonEvent(int button, int action);
    void scrollEvent(double distance);
    void windowResizeEvent(int width, int height);
    void framebufferResizeEvent(int width, int height);

private:
    std::unordered_map<std::string, std::shared_ptr<Screen>> screens;
    std::shared_ptr<Screen> activeScreen;
};