#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 UIColor;

void main()
{    
    // vec4 sampled = vec4(1.0, 1.0, 1.0, texture(sprite, TexCoords).r);
    color = vec4(UIColor, 1.0);
}  