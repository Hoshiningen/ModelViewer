#pragma once

#include "Light/ILight.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/vec3.hpp>

class ShaderProgram;

class DirectionalLight : public ILight {
public:
    DirectionalLight();

    virtual void apply(ShaderProgram* pShader) const override;
    virtual void apply(ShaderProgram* pShader, std::size_t index) const override;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DECLARE_GETTER_IMMUTABLE_COPY(pitch, float)
    DECLARE_SETTER_COPY(pitch, float)

    DECLARE_GETTER_IMMUTABLE_COPY(yaw, float)
    DECLARE_SETTER_COPY(yaw, float)

    DECLARE_GETTER_IMMUTABLE(direction, glm::vec3)

    DECLARE_GETTER_IMMUTABLE(color, glm::vec3)
    DECLARE_SETTER_CONSTREF(color, glm::vec3)

    DECLARE_GETTER_IMMUTABLE_COPY(intensity, float)
    DECLARE_SETTER_COPY(intensity, float)

    DECLARE_GETTER_IMMUTABLE_COPY(enabled, bool)
    DECLARE_SETTER_COPY(enabled, bool)

private:
    COMPILATION_FIREWALL_COPY_MOVE(DirectionalLight)
};