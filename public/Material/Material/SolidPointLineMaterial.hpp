#pragma once

#include "Material/IMaterial.hpp"

#include <memory>

#include <glm/vec3.hpp>

class ShaderProgram;

class SolidPointLineMaterial : public IMaterial {
public:
    SolidPointLineMaterial();
    virtual ~SolidPointLineMaterial() noexcept;

    SolidPointLineMaterial(const SolidPointLineMaterial& other);
    SolidPointLineMaterial& operator=(const SolidPointLineMaterial& other);

    SolidPointLineMaterial(SolidPointLineMaterial&& other) noexcept;
    SolidPointLineMaterial& operator=(SolidPointLineMaterial&& other) noexcept;

    virtual void apply(ShaderProgram* pShader) const override;

    void color(const glm::vec3& value);
    virtual void wireframe(bool value) override {};

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
