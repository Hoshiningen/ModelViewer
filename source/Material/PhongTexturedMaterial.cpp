#include "Material/PhongTexturedMaterial.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Texture/Texture.hpp"

#include <optional>

#include <glad/glad.h>

struct PhongTexturedMaterial::Private {
    std::optional<Texture> m_diffuseMap;
    std::optional<Texture> m_specularMap;
    std::optional<Texture> m_emissiveMap;

    float m_shininess = 28.f;

    float m_ambientIntensity = 1.f;
    float m_diffuseIntensity = 1.f;
    float m_emissiveIntensity = 1.f;
    float m_specularIntensity = 1.f;

    bool m_wireframe = false;
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

    pShader->set("material.ambientIntensity", m_pPrivate->m_ambientIntensity);

    if (m_pPrivate->m_diffuseMap.has_value() && m_pPrivate->m_diffuseMap->initialized()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(static_cast<GLenum>(m_pPrivate->m_diffuseMap->target()), m_pPrivate->m_diffuseMap->id());

        pShader->set("material.hasDiffuse", true);
        pShader->set("material.diffuseMap", 0);
        pShader->set("material.diffuseIntensity", m_pPrivate->m_diffuseIntensity);
    }
    else
        pShader->set("material.hasDiffuse", false);

    if (m_pPrivate->m_emissiveMap.has_value() && m_pPrivate->m_emissiveMap->initialized()) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(static_cast<GLenum>(m_pPrivate->m_emissiveMap->target()), m_pPrivate->m_emissiveMap->id());
        
        pShader->set("material.hasEmissive", true);
        pShader->set("material.emissiveMap", 1);
        pShader->set("material.emissiveIntensity", m_pPrivate->m_emissiveIntensity);
    }
    else
        pShader->set("material.hasEmissive", false);

    if (m_pPrivate->m_specularMap.has_value() && m_pPrivate->m_specularMap->initialized()) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(static_cast<GLenum>(m_pPrivate->m_specularMap->target()), m_pPrivate->m_specularMap->id());
        
        pShader->set("material.hasSpecular", true);
        pShader->set("material.specularMap", 2);
        pShader->set("material.specularIntensity", m_pPrivate->m_specularIntensity);
        pShader->set("material.shininess", m_pPrivate->m_shininess);
    }
    else
        pShader->set("material.hasSpecular", false);

    pShader->set("wireframe", m_pPrivate->m_wireframe);
}

void PhongTexturedMaterial::wireframe(bool value) {
    m_pPrivate->m_wireframe = value;
}

void PhongTexturedMaterial::diffuseMap(const Texture& value) {
    m_pPrivate->m_diffuseMap = value;
}

const std::optional<Texture>& PhongTexturedMaterial::diffuseMap() const {
    return m_pPrivate->m_diffuseMap;
}

std::optional<Texture>& PhongTexturedMaterial::diffuseMap() {
    return m_pPrivate->m_diffuseMap;
}

void PhongTexturedMaterial::specularMap(const Texture& value) {
    m_pPrivate->m_specularMap = value;
}

const std::optional<Texture>& PhongTexturedMaterial::specularMap() const {
    return m_pPrivate->m_specularMap;
}

std::optional<Texture>& PhongTexturedMaterial::specularMap() {
    return m_pPrivate->m_specularMap;
}

void PhongTexturedMaterial::emissiveMap(const Texture& value) {
    m_pPrivate->m_emissiveMap = value;
}

const std::optional<Texture>& PhongTexturedMaterial::emissiveMap() const {
    return m_pPrivate->m_emissiveMap;
}

std::optional<Texture>& PhongTexturedMaterial::emissiveMap() {
    return m_pPrivate->m_emissiveMap;
}

void PhongTexturedMaterial::shininess(float value) {
    m_pPrivate->m_shininess = value;
}

void PhongTexturedMaterial::ambientIntensity(float value) {
    m_pPrivate->m_ambientIntensity = value;
}

void PhongTexturedMaterial::diffuseIntensity(float value) {
    m_pPrivate->m_diffuseIntensity = value;
}

void PhongTexturedMaterial::emissiveIntensity(float value) {
    m_pPrivate->m_emissiveIntensity = value;
}

void PhongTexturedMaterial::specularIntensity(float value) {
    m_pPrivate->m_specularIntensity = value;
}
