#pragma once

#include "Common/ClassMacros.hpp"

#include <forward_list>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera;
class DirectionalLight;
class IMaterial;
class ShaderProgram;
class Texture;
class VertexBuffered;

class Renderer {
public:
    Renderer();

    void setup();
    void camera(Camera* pCamera);

    void draw(const VertexBuffered& geometry, const IMaterial& material) const;
    void draw(const std::forward_list<VertexBuffered>& model, const IMaterial& material) const;

    void onTextureLoaded(const Texture& texture) const;
    void onModelLoaded(const IMaterial* pMaterial, std::forward_list<VertexBuffered>* pModel) const;
    void onLightChanged(const DirectionalLight& light, uint8_t index, bool enabled) const;
    void onAmbientColorChanged(const glm::vec3& color, float intensity) const;

private:
    COMPILATION_FIREWALL(Renderer)
};
