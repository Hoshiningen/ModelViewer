#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 objectColor;

uniform float ambientIntensity;
uniform float specularIntensity;
uniform float specularShininess;

uniform DirectionalLight directionalLight;
uniform vec3 eyePoint;

void main() {
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 eyeDir = normalize(eyePoint - fragPosition);
    vec3 normal = normalize(fragNormal);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambientColor = directionalLight.ambient * ambientIntensity;

    float diffuseScalar = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuseColor = directionalLight.diffuse * diffuseScalar;

    float specularScalar = pow(max(dot(eyeDir, reflectDir), 0.0), specularShininess);
    vec3 specularColor = specularIntensity * specularScalar * directionalLight.specular;

    fragColor = vec4(vec3((ambientColor + diffuseColor + specularColor) * objectColor), 1.0);
}