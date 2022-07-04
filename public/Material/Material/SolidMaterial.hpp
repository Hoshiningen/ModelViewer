#pragma once

#include "Material/IMaterial.hpp"

#include <memory>

#include <glm/vec3.hpp>

class ShaderProgram;

class SolidMaterial : public IMaterial {
public:
    SolidMaterial();
    virtual ~SolidMaterial() noexcept;

    SolidMaterial(const SolidMaterial& other);
    SolidMaterial& operator=(const SolidMaterial& other);

    SolidMaterial(SolidMaterial&& other) noexcept;
    SolidMaterial& operator=(SolidMaterial&& other) noexcept;

    virtual void apply(ShaderProgram* pShader) const override;

    void color(const glm::vec3& value);
    virtual void wireframe(bool value) override {}

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
