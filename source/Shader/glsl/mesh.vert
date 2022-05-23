#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(position, 1.0f);
    
    fragNormal = normal;
    fragPosition = vec3(model * vec4(position, 1.0f));
}