#pragma once

#include "Material/IMaterial.hpp"

#include "Common/ClassMacros.hpp"

#include <optional>

#include <glm/vec3.hpp>

class Texture;

class PhongTexturedMaterial : public IMaterial {
public:
    PhongTexturedMaterial();

    virtual void apply(ShaderProgram* pShader) const override;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DECLARE_GETTER_CONST_CORRECT(diffuseMap, std::optional<Texture>)
    DECLARE_SETTER_CONSTREF(diffuseMap, Texture)

    DECLARE_GETTER_CONST_CORRECT(specularMap, std::optional<Texture>)
    DECLARE_SETTER_CONSTREF(specularMap, Texture)

    DECLARE_GETTER_CONST_CORRECT(emissiveMap, std::optional<Texture>)
    DECLARE_SETTER_CONSTREF(emissiveMap, Texture)

    DECLARE_GETTER_MUTABLE(shininess, float)
    DECLARE_SETTER_COPY(shininess, float)

    DECLARE_GETTER_MUTABLE(diffuseIntensity, float)
    DECLARE_SETTER_COPY(diffuseIntensity, float)

    DECLARE_GETTER_MUTABLE(emissiveIntensity, float)
    DECLARE_SETTER_COPY(emissiveIntensity, float)

    DECLARE_GETTER_MUTABLE(specularIntensity, float)
    DECLARE_SETTER_COPY(specularIntensity, float)

private:
    COMPILATION_FIREWALL_COPY_MOVE(PhongTexturedMaterial)
};
