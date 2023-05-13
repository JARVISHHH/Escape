#version 330 core

// All in object space
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 color;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

uniform mat4 model, view, projection;

uniform int vertNormalSource;

uniform mat4 lightSpaceMatrix[16];

out vec3 worldSpace_pos;
out vec3 particular_norm;
out vec2 tex_coord;
out vec3 vertColor;

out vec4 lightSpace_pos[16];

out mat3 TBN;

void main() {
    worldSpace_pos = vec3(model * vec4(pos, 1.0));
    if(vertNormalSource == 1) {
        vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
        vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(norm, 0.0)));
        TBN = mat3(T, B, N);
    }
    particular_norm = vec3(transpose(inverse(model))*vec4(norm, 0.0));
    tex_coord = uv;
    vertColor = color;

    gl_Position = projection*view*model*vec4(pos, 1.0);

    for(int i = 0; i < 16; i++)
        lightSpace_pos[i] = lightSpaceMatrix[i] * model * vec4(pos, 1.0);


}