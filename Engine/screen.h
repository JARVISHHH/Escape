#pragma once

#include <unordered_map>

#include "Graphics/global.h"
#include <Engine/Mesh/triangle.h>
#include "application.h"

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class GameWorld;
class UIElement;

class Screen : public std::enable_shared_from_this<Screen>
{
public:
    static std::unordered_map<int, bool> keyPressing;
    static glm::vec2 mousePos;
    static std::unordered_map<int, bool> mousePressing;
    static std::shared_ptr<Application> application;

    virtual void init();
    static std::shared_ptr<Application> getApp();
    void addEnvironmentMesh(std::string name, std::string path, bool hasUV = true, int uvScale = 1);
    std::vector<std::shared_ptr<Triangle>> getEnvironmentMesh(std::string name);

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
    std::unordered_map<std::string, std::vector<std::shared_ptr<Triangle>>> meshTriangles;
    std::shared_ptr<UIElement> ui;

    bool active = true;
    int width, height;
};