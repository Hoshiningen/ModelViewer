#include "Material/PhongTexturedMaterial.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Texture/Texture.hpp"

#include <optional>

#include <glad/glad.h>

struct PhongTexturedMaterial::Private {
    std::optional<Texture> diffuseMap;
    std::optional<Texture> emissiveMap;
    std::optional<Texture> specularMap;

    float shininess = 28.f;

    float diffuseIntensity = 1.f;
    float emissiveIntensity = 1.f;
    float specularIntensity = 1.f;
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhongTexturedMaterial::Private,
        shininess,
        diffuseIntensity,
        emissiveIntensity,
        specularIntensity
    )
};

PhongTexturedMaterial::PhongTexturedMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

PhongTexturedMaterial::~PhongTexturedMaterial() noexcept {}

PhongTexturedMaterial::PhongTexturedMaterial(const PhongTexturedMaterial& other) {
    *this = other;
}

PhongTexturedMaterial& PhongTexturedMaterial::operator=(const PhongTexturedMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

PhongTexturedMaterial::PhongTexturedMaterial(PhongTexturedMaterial&& other) noexcept {
    *this = std::move(other);
}

PhongTexturedMaterial& PhongTexturedMaterial::operator=(PhongTexturedMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void PhongTexturedMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    pShader->set("phongMaterial.isMapped", true);

    if (m_pPrivate->diffuseMap.has_value() && m_pPrivate->diffuseMap->initialized()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(static_cast<GLenum>(m_pPrivate->diffuseMap->target()), m_pPrivate->diffuseMap->id());

        pShader->set("phongMaterial.hasDiffuse", true);
        pShader->set("phongMaterial.diffuseMap", 0);
        pShader->set("phongMaterial.diffuseIntensity", m_pPrivate->diffuseIntensity);
    }
    else
        pShader->set("phongMaterial.hasDiffuse", false);

    if (m_pPrivate->emissiveMap.has_value() && m_pPrivate->emissiveMap->initialized()) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(static_cast<GLenum>(m_pPrivate->emissiveMap->target()), m_pPrivate->emissiveMap->id());
        
        pShader->set("phongMaterial.hasEmissive", true);
        pShader->set("phongMaterial.emissiveMap", 1);
        pShader->set("phongMaterial.emissiveIntensity", m_pPrivate->emissiveIntensity);
    }
    else
        pShader->set("phongMaterial.hasEmissive", false);

    if (m_pPrivate->specularMap.has_value() && m_pPrivate->specularMap->initialized()) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(static_cast<GLenum>(m_pPrivate->specularMap->target()), m_pPrivate->specularMap->id());
        
        pShader->set("phongMaterial.hasSpecular", true);
        pShader->set("phongMaterial.specularMap", 2);
        pShader->set("phongMaterial.specularIntensity", m_pPrivate->specularIntensity);
        pShader->set("phongMaterial.shininess", m_pPrivate->shininess);
    }
    else
        pShader->set("phongMaterial.hasSpecular", false);
}

std::string_view PhongTexturedMaterial::id() const {
    return "PhongTexturedMaterial";
}

nlohmann::json PhongTexturedMaterial::save() const {

    nlohmann::json json;
    json[id().data()] = *m_pPrivate;

    return json;
}

void PhongTexturedMaterial::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    *m_pPrivate = settings;
}

DEFINE_GETTER_CONST_CORRECT(PhongTexturedMaterial, diffuseMap, std::optional<Texture>, m_pPrivate->diffuseMap)
DEFINE_SETTER_CONSTREF_EXPLICIT(PhongTexturedMaterial, diffuseMap, Texture, m_pPrivate->diffuseMap)

DEFINE_GETTER_CONST_CORRECT(PhongTexturedMaterial, emissiveMap, std::optional<Texture>, m_pPrivate->emissiveMap)
DEFINE_SETTER_CONSTREF_EXPLICIT(PhongTexturedMaterial, emissiveMap, Texture, m_pPrivate->emissiveMap)

DEFINE_GETTER_CONST_CORRECT(PhongTexturedMaterial, specularMap, std::optional<Texture>, m_pPrivate->specularMap)
DEFINE_SETTER_CONSTREF_EXPLICIT(PhongTexturedMaterial, specularMap, Texture, m_pPrivate->specularMap)

DEFINE_GETTER_MUTABLE(PhongTexturedMaterial, shininess, float, m_pPrivate->shininess)
DEFINE_SETTER_COPY(PhongTexturedMaterial, shininess, m_pPrivate->shininess)

DEFINE_GETTER_MUTABLE(PhongTexturedMaterial, diffuseIntensity, float, m_pPrivate->diffuseIntensity)
DEFINE_SETTER_COPY(PhongTexturedMaterial, diffuseIntensity, m_pPrivate->diffuseIntensity)

DEFINE_GETTER_MUTABLE(PhongTexturedMaterial, emissiveIntensity, float, m_pPrivate->emissiveIntensity)
DEFINE_SETTER_COPY(PhongTexturedMaterial, emissiveIntensity, m_pPrivate->emissiveIntensity)

DEFINE_GETTER_MUTABLE(PhongTexturedMaterial, specularIntensity, float, m_pPrivate->specularIntensity)
DEFINE_SETTER_COPY(PhongTexturedMaterial, specularIntensity, m_pPrivate->specularIntensity)
