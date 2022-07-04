#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out FragmentData {
    vec3 position;
    vec3 normal;
    vec4 color;
} vertOut;

struct Matrices {
    mat4 model;
    mat4 viewProjection;
};

uniform Matrices matrices;

void main() {

    gl_Position = matrices.viewProjection * matrices.model * vec4(position, 1.0f);
    
    vertOut.normal = normalize(normal);
    vertOut.position = vec3(matrices.model * vec4(position, 1.0f));
    vertOut.color = color;
}
