#pragma once

#include "Material/IMaterial.hpp"

#include <memory>

#include <glm/vec4.hpp>

class ShaderProgram;

class PhongMaterial : public IMaterial {
public:
    PhongMaterial();
    virtual ~PhongMaterial() noexcept;

    PhongMaterial(const PhongMaterial& other);
    PhongMaterial& operator=(const PhongMaterial& other);

    PhongMaterial(PhongMaterial&& other) noexcept;
    PhongMaterial& operator=(PhongMaterial&& other) noexcept;

    virtual void apply(ShaderProgram* pShader) const override;

    void ambientColor(const glm::vec4& value);
    void ambientIntensity(float value);

    void diffuseColor(const glm::vec4& value);
    void diffuseIntensity(float value);
    
    void specularColor(const glm::vec4& value);
    void specularIntensity(float value);

    void shininess(float value);

    virtual void wireframe(bool value) override;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
