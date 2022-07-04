#pragma once

#include "Material/IMaterial.hpp"

#include <memory>

#include <glm/vec3.hpp>

class ShaderProgram;

class LambertianMaterial : public IMaterial {
public:
    LambertianMaterial();
    virtual ~LambertianMaterial() noexcept;

    LambertianMaterial(const LambertianMaterial& other);
    LambertianMaterial& operator=(const LambertianMaterial& other);

    LambertianMaterial(LambertianMaterial&& other) noexcept;
    LambertianMaterial& operator=(LambertianMaterial&& other) noexcept;

    virtual void apply(ShaderProgram* pShader) const override;

    void diffuseIntensity(float value);
    void diffuseColor(const glm::vec3& value);

    virtual void wireframe(bool value) override;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};