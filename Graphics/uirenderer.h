#pragma once

// Credit to https://learnopengl.com/In-Practice/Text-Rendering

#include "font.h"
#include <string>
#include <memory>

class UIElement;

enum class AnchorPoint{
    TopLeft, TopCenter, TopRight
};

class UIRenderer{
public:
    void initialize();
    void renderUIText(std::shared_ptr<Font> font, std::string text, glm::vec2 anchorPosition, AnchorPoint anchorPoint, float textBoxWidth, float fontSize, float lineSpacing, glm::vec3 textColor);
    void renderUI(std::shared_ptr<UIElement> uiElement);

private:
    GLuint m_vao;
    GLuint m_vbo;
};