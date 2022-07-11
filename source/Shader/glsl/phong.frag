#version 330 core

in FragmentData {
    vec3 position;
    vec3 normal;
    vec4 color;
} fragIn;

out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform DirectionalLight directionalLight;

struct Material {
    vec4 ambientColor;
    float ambientIntensity;

    vec4 diffuseColor;
    float diffuseIntensity;

    vec4 specularColor;
    float specularIntensity;
    
    float shininess;
};

uniform Material material;
uniform vec3 eyePoint;

uniform bool includeAmbient = false;
uniform bool includeDiffuse = false;
uniform bool includeSpecular = false;

uniform bool wireframe = false;
uniform bool hasVertexColor = false;

void main() {
 
    // global ambient color
    const vec3 ambientColor = vec3(0.1f, 0.1f, 0.1f);

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

    vec3 scatteredLight; // ambient and diffuse light
    if (includeAmbient && !wireframe)
        scatteredLight = ambientColor * material.ambientColor.rgb * material.ambientIntensity;

    if (includeDiffuse && !wireframe) {
        scatteredLight += directionalLight.color * lightAngle * directionalLight.intensity;
        scatteredLight *= material.diffuseColor.rgb * material.diffuseIntensity;

        if (hasVertexColor)
            scatteredLight *= fragIn.color.rgb;
    }

    vec3 reflectedLight; // specular highlights
    if (includeSpecular && !wireframe) {
        reflectedLight = directionalLight.color * specularAngle * directionalLight.intensity;
        reflectedLight *= material.specularColor.rgb * material.specularIntensity;
    }

    // Final pixel color
    vec3 pixelColor;
    if ((wireframe || (!includeAmbient && !includeDiffuse && !includeSpecular)) && hasVertexColor)
        pixelColor = fragIn.color.rgb;
    else
        pixelColor = min(scatteredLight + reflectedLight, vec3(1.f));

    fragColor = vec4(pixelColor, hasVertexColor ? fragIn.color.a : 1.f);
}
