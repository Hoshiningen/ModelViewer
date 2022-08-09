#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texel;

out FragmentData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texel;
} vertOut;

struct Matrices {
    mat4 model;
    mat4 viewProjection;
};

uniform Matrices matrices;

void main() {

    gl_Position = matrices.viewProjection * matrices.model * vec4(position, 1.0f);
    
    vertOut.normal = mat3(transpose(inverse(matrices.model))) * normalize(normal);
    vertOut.position = vec3(matrices.model * vec4(position, 1.0f));
    vertOut.color = color;
    vertOut.texel = texel;
}
