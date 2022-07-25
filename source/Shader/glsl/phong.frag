#version 330 core

// =============== Pipeline ================
in FragmentData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texel;
} fragIn;

out vec4 fragColor;
// =============== Pipeline ================

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PhongMaterial {
    float ambientIntensity;
    float emissiveIntensity;
    float diffuseIntensity;
    float specularIntensity;

    vec4 ambientColor;
    vec4 specularColor;
    vec4 diffuseColor;

    sampler2D diffuseMap;
    sampler2D emissiveMap;
    sampler2D specularMap;

    float shininess;

    bool hasAmbient;
    bool hasDiffuse;
    bool hasEmissive;
    bool hasSpecular;
    bool isMapped;
};

const int kMaxLights = 3;

// =============== Uniforms ================
uniform DirectionalLight directionalLights[kMaxLights];
uniform bool enabledLights[kMaxLights];

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform PhongMaterial phongMaterial;
uniform vec3 eyePoint;
uniform bool hasVertexColor = false;
// =============== Uniforms ================

vec3 AmbientColor() {

    vec3 color = ambientColor * ambientIntensity;
    if (phongMaterial.hasAmbient)
        color += phongMaterial.isMapped ? vec3(0.f) : phongMaterial.ambientColor.rgb * phongMaterial.ambientIntensity;

    return color;
}

vec3 DiffuseColor() {
    return phongMaterial.isMapped ? texture(phongMaterial.diffuseMap, fragIn.texel).rgb : phongMaterial.diffuseColor.rgb;
}

vec3 EmissiveColor() {
    return phongMaterial.isMapped ? texture(phongMaterial.emissiveMap, fragIn.texel).rgb : vec3(0.f);
}

vec3 SpecularColor() {
    return phongMaterial.isMapped ? texture(phongMaterial.specularMap, fragIn.texel).rgb : phongMaterial.specularColor.rgb;
}

vec3 ComputeScatteredLight(float lightAngle, int lightIndex) {

    vec3 color = vec3(0.f);
    if (phongMaterial.hasDiffuse) {
        vec3 lightColor = directionalLights[lightIndex].color * lightAngle * directionalLights[lightIndex].intensity;
        color = lightColor * DiffuseColor() * phongMaterial.diffuseIntensity;    
    }

    return color;
}

vec3 ComputeReflectedLight(float specularAngle, int lightIndex) {

    vec3 color = vec3(0.f);
    if (phongMaterial.hasSpecular) {
        vec3 lightColor = directionalLights[lightIndex].color * specularAngle * directionalLights[lightIndex].intensity;
        color = lightColor * SpecularColor() * phongMaterial.specularIntensity;
    }

    return color;
}

vec3 ComputeDirectionalLighting(int lightIndex) {

    vec3 lightDir = normalize(-directionalLights[lightIndex].direction);
    vec3 normal = normalize(fragIn.normal);
    vec3 eyeDir = normalize(eyePoint - fragIn.position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float lightAngle = max(dot(normal, lightDir), 0.f);
    float specularAngle = max(dot(eyeDir, reflectDir), 0.f);

    if (lightAngle == 0.f)
        specularAngle = 0.f;
    else
        specularAngle = pow(specularAngle, phongMaterial.shininess);

    vec3 pixelColor = vec3(0.f);
    if (phongMaterial.hasDiffuse) {
        vec3 scatteredLight = ComputeScatteredLight(lightAngle, lightIndex);
        pixelColor = hasVertexColor ? fragIn.color.rgb * scatteredLight : scatteredLight;
    }

    if (phongMaterial.hasEmissive) {
        vec3 emittedLight = EmissiveColor() * phongMaterial.emissiveIntensity;
        pixelColor += emittedLight;
    }

    if (phongMaterial.hasSpecular) {
        vec3 reflectedLight = ComputeReflectedLight(specularAngle, lightIndex);
        pixelColor += reflectedLight;
    }

    return min(pixelColor, vec3(1.f));
}

void main() {
 
    vec3 pixelColor = AmbientColor();
    for (int lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {

        if (!enabledLights[lightIndex])
            continue;

        pixelColor += ComputeDirectionalLighting(lightIndex);
    }

    fragColor = vec4(pixelColor, hasVertexColor ? fragIn.color.a : 1.f);
}
