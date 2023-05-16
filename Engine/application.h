#pragma once

#include <unordered_map>

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

class Screen;

class Application: std::enable_shared_from_this<Application>
{
public:
    Application();
    ~Application();

    virtual void start();

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

    int getWidth();
    int getHeight();

protected:
    std::unordered_map<std::string, std::shared_ptr<Screen>> screens;
    std::shared_ptr<Screen> activeScreen;

    int width = 640, height = 480;
};