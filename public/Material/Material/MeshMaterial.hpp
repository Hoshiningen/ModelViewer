#pragma once

#include "Material/IMaterial.hpp"

#include <memory>
#include <optional>

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
    virtual void wireframe(bool value) override;

    void diffuseMap(const Texture& value);
    const std::optional<Texture>& diffuseMap() const;
    std::optional<Texture>& diffuseMap();

    void specularMap(const Texture& value);
    const std::optional<Texture>& specularMap() const;
    std::optional<Texture>& specularMap();

    void emissiveMap(const Texture& value);
    const std::optional<Texture>& emissiveMap() const;
    std::optional<Texture>& emissiveMap();

    void shininess(float value);

    void ambientIntensity(float value);
    void diffuseIntensity(float value);
    void emissiveIntensity(float value);
    void specularIntensity(float value);

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};