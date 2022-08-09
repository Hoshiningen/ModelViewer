#pragma once

#include "Common/ClassMacros.hpp"

#include <forward_list>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera;
class DirectionalLight;
class IMaterial;
class Mesh;
class ShaderProgram;
class Texture;
class VertexBuffered;

class Renderer {
public:
    Renderer();

    void setup();
    void camera(Camera* pCamera);

    void draw(const Mesh& mesh) const;

    void initializeMesh(Mesh& mesh) const;

    void onTextureLoaded(const Texture& texture) const;

    void directionalLight(DirectionalLight** ppLight, uint8_t lightIndex);
    void ambientColor(glm::vec3* pAmbientColor, float* pAmbientIntensity);

private:
    COMPILATION_FIREWALL(Renderer)
};
