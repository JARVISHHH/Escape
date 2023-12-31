#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform int renderType;
uniform sampler2D sprite;
uniform vec3 UIColor;

void main()
{    
    if(renderType == 0) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(sprite, TexCoords).r);
        color = vec4(UIColor, 1.0) * sampled;
        // float a = texture(sprite, TexCoords).r;
        // color = vec4(a, a, a, 1.0);
    }
    else if(renderType == 1) {
        color = vec4(UIColor, 1.0);
    }
    else if(renderType == 2) {
        if(texture(sprite, TexCoords).a < 1) {
        color = vec4(1.0, 1.0, 1.0, 1.0);
        }
        else {
            color = vec4(1.0, 1.0, 1.0, texture(sprite, TexCoords).a);
        }
        color = vec4(texture(sprite, TexCoords).xyz, 0.5);
        color = texture(sprite, TexCoords);
        // float a = texture(sprite, TexCoords).a;
        // color = vec4(a, a, a, 1.0);
    }
}  