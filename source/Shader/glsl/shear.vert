#version 330 core

layout (location = 0) in vec3 basisVector;

out vec4 color;

uniform mat4 shear;
uniform mat4 viewProjection;
uniform vec4 lineColor;

void main() {
    gl_Position = viewProjection * shear * vec4(basisVector, 1);
    color = lineColor;
}
