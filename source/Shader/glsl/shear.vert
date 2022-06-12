#version 330 core

layout (location = 0) in vec3 basisVector;

uniform mat4 shear;

struct Matrices {
    mat4 viewProjection;
    mat4 shear;
};

uniform Matrices matrices;

void main() {
    gl_Position = matrices.viewProjection * matrices.shear * vec4(basisVector, 1);
}
