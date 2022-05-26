#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out FragmentData {
    vec3 position;
    vec3 normal;
} vertOut;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(position, 1.0f);
    
    vertOut.normal = normalize(normal);
    vertOut.position = vec3(model * vec4(position, 1.0f));
}