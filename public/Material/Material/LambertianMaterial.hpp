#pragma once

#include "Material/IMaterial.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/vec4.hpp>

class ShaderProgram;

class LambertianMaterial : public IMaterial {
public:
    LambertianMaterial();

    virtual void apply(ShaderProgram* pShader) const override;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DECLARE_GETTER_MUTABLE(diffuseIntensity, float)
    DECLARE_SETTER_COPY(diffuseIntensity, float)

    DECLARE_GETTER_MUTABLE(diffuseColor, glm::vec4)
    DECLARE_SETTER_CONSTREF(diffuseColor, glm::vec4)

private:
    COMPILATION_FIREWALL_COPY_MOVE(LambertianMaterial)
};
