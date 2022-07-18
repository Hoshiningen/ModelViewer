#pragma once

#include "Light/ILight.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/vec3.hpp>

class ShaderProgram;

class DirectionalLight : public ILight {
public:
    DirectionalLight();

    virtual void apply(ShaderProgram* pShader) const override;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DECLARE_GETTER_MUTABLE(direction, glm::vec3)
    DECLARE_SETTER_CONSTREF(direction, glm::vec3)

    DECLARE_GETTER_MUTABLE(color, glm::vec3)
    DECLARE_SETTER_CONSTREF(color, glm::vec3)

    DECLARE_GETTER_MUTABLE(intensity, float)
    DECLARE_SETTER_COPY(intensity, float)

private:
    COMPILATION_FIREWALL_COPY_MOVE(DirectionalLight)
};