#pragma once

#include "GLWrappers/texture.h"
#include <memory>

#include "glm/glm.hpp"

enum ColorSource{
    SOLID_COLOR,
    TEXTURE_COLOR,
    PER_VERTEX_COLOR
};

enum NormalSource {
    ORIGINAL_NORMAL,
    TEXTURE_NORMAL
};

class Material{
public:
    Material(glm::vec3 color, float shininess = 1.f); //Constructor for setting SOLID_COLOR based material
    Material(float shininess = 1.f); //Constructor for setting PER_VERTEX_COLOR based material
    Material(std::shared_ptr<Texture> texture, float shininess = 1.f); //Constructor for setting TEXTURE_COLOR based material
    ~Material();

    // Functions for setting uniforms
    ColorSource getColorSource();
    void setColorSource(ColorSource source);
    NormalSource getNormalSource();
    void setNormalSource(NormalSource source);
    std::shared_ptr<Texture> getTexture();
    void setTexture(std::shared_ptr<Texture> texture);
    glm::vec3 getColor();
    void setColor(glm::vec3 color);
    float getShininess();
    void setShininess(float shininess);

private:
    ColorSource m_colorSource;
    NormalSource m_normalSource;
    std::shared_ptr<Texture> m_texture;
    glm::vec3 m_color;
    float m_shininess;
};