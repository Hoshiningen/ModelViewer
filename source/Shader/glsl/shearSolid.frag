#version 330 core

out vec4 fragColor;

struct Material {
    vec3 color;
};

uniform Material material;

void main() {
    fragColor = vec4(material.color, 1.f);
}
