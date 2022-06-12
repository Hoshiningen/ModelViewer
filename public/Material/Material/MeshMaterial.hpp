#pragma once

#include "Material/IMaterial.hpp"

#include <memory>

#include <glm/vec3.hpp>

class Texture;

class MeshMaterial : public IMaterial{
public:
    MeshMaterial();
    virtual ~MeshMaterial() noexcept;

    MeshMaterial(const MeshMaterial& other);
    MeshMaterial& operator=(const MeshMaterial& other);

    MeshMaterial(MeshMaterial&& other) noexcept;
    MeshMaterial& operator=(MeshMaterial&& other) noexcept;

    virtual void apply(ShaderProgram* pShader) const override;

    void diffuseMap(const Texture& value);
    void specularMap(const Texture& value);
    void emissiveMap(const Texture& value);

    void specularShininess(float value);

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};