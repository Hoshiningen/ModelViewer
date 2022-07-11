#pragma once

#include <forward_list>
#include <memory>

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
    virtual ~Renderer();

    void setup();
    void camera(Camera* pCamera);

    void draw(const VertexBuffered& geometry, const IMaterial& material) const;
    void draw(const std::forward_list<VertexBuffered>& model, const IMaterial& material) const;

    void onTextureLoaded(const Texture& texture) const;
    void onModelLoaded(const IMaterial* pMaterial, std::forward_list<VertexBuffered>* pModel) const;
    void onLightChanged(const DirectionalLight& light) const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
