#version 330 core

in FragmentData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texel;
} fragIn;

out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform DirectionalLight directionalLight;

struct Material {
    float ambientIntensity;

    sampler2D diffuseMap;
    float diffuseIntensity;
    bool hasDiffuse;

    sampler2D emissiveMap;
    float emissiveIntensity;
    bool hasEmissive;

    sampler2D specularMap;
    float specularIntensity;
    bool hasSpecular;

    float shininess;
};

uniform Material material;
uniform vec3 eyePoint;

uniform bool wireframe = false;
uniform bool hasVertexColor = false;

vec3 ComputeScatteredLight(float lightAngle) {

    // global ambient color
    const vec3 ambientColor = vec3(0.1f, 0.1f, 0.1f);
    vec3 color = ambientColor * material.ambientIntensity;

    if (material.hasDiffuse) {
        vec3 diffuseColor = directionalLight.color * lightAngle * directionalLight.intensity;
        diffuseColor *= texture(material.diffuseMap, fragIn.texel).rgb * material.diffuseIntensity;
        
        color += diffuseColor;
    }

    return color;
}

vec3 ComputeReflectedLight(float specularAngle) {

    vec3 color;
    if (material.hasSpecular) {
        color = directionalLight.color * specularAngle * directionalLight.intensity;
        color *= texture(material.specularMap, fragIn.texel).rgb * material.specularIntensity;
    }

    return color;
}

void main() {
 
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 normal = normalize(fragIn.normal);
    vec3 eyeDir = normalize(eyePoint - fragIn.position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float lightAngle = max(dot(normal, lightDir), 0.f);
    float specularAngle = max(dot(eyeDir, reflectDir), 0.f);

    if (lightAngle == 0.f)
        specularAngle = 0.f;
    else
        specularAngle = pow(specularAngle, material.shininess);

    vec3 pixelColor;
    if (wireframe) {
        pixelColor = hasVertexColor ? fragIn.color.rgb : vec3(1.f, 1.f, 1.f);
    }
    else {
        vec3 scatteredLight = ComputeScatteredLight(lightAngle);
        pixelColor = hasVertexColor ? fragIn.color.rgb * scatteredLight : scatteredLight;

        if (material.hasEmissive) {
            vec3 emittedLight = texture(material.emissiveMap, fragIn.texel).rgb * material.emissiveIntensity;
            pixelColor += emittedLight;
        }

        if (material.hasSpecular) {
            vec3 reflectedLight = ComputeReflectedLight(specularAngle);
            pixelColor += reflectedLight;
        }
    }

    pixelColor = min(pixelColor, vec3(1.f));
    fragColor = vec4(pixelColor, hasVertexColor ? fragIn.color.a : 1.f);
}
