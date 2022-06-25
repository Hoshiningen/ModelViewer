#include "Material/PhongMaterial.hpp"
#include "Shader/ShaderProgram.hpp"

struct PhongMaterial::Private {
    glm::vec3 m_ambientColor{ 1.f, 1.f, 1.f };
    float m_ambientIntensity = 1.f;

    glm::vec3 m_diffuseColor{ 1.f, 1.f, 1.f };
    float m_diffuseIntensity = 1.f;

    glm::vec3 m_specularColor{ 1.f, 1.f, 1.f };
    float m_specularIntensity = 1.f;

    float m_shininess = 32.f;

    bool m_wireframe = false;
};


PhongMaterial::PhongMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

PhongMaterial::~PhongMaterial() noexcept {}

PhongMaterial::PhongMaterial(const PhongMaterial& other) {
    *this = other;
}

PhongMaterial& PhongMaterial::operator=(const PhongMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

PhongMaterial::PhongMaterial(PhongMaterial&& other) noexcept {
    *this = std::move(other);
}

PhongMaterial& PhongMaterial::operator=(PhongMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void PhongMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    pShader->set("material.diffuseIntensity", m_pPrivate->m_diffuseIntensity);
    pShader->set("material.ambientIntensity", m_pPrivate->m_ambientIntensity);
    pShader->set("material.specularIntensity", m_pPrivate->m_specularIntensity);
    pShader->set("material.shininess", m_pPrivate->m_shininess);
    pShader->set("material.ambientColor", m_pPrivate->m_ambientColor);
    pShader->set("material.diffuseColor", m_pPrivate->m_diffuseColor);
    pShader->set("material.specularColor", m_pPrivate->m_specularColor);

    pShader->set("includeAmbient", true);
    pShader->set("includeDiffuse", true);
    pShader->set("includeSpecular", true);

    pShader->set("wireframe", m_pPrivate->m_wireframe);
}

void PhongMaterial::ambientColor(const glm::vec3& value) {
    m_pPrivate->m_ambientColor = value;
}

void PhongMaterial::ambientIntensity(float value) {
    m_pPrivate->m_ambientIntensity = value;
}

void PhongMaterial::diffuseColor(const glm::vec3& value) {
    m_pPrivate->m_diffuseColor = value;
}

void PhongMaterial::diffuseIntensity(float value){
    m_pPrivate->m_diffuseIntensity = value;
}

void PhongMaterial::specularColor(const glm::vec3& value) {
    m_pPrivate->m_specularColor = value;
}

void PhongMaterial::specularIntensity(float value) {
    m_pPrivate->m_specularIntensity = value;
}

void PhongMaterial::shininess(float value) {
    m_pPrivate->m_shininess = value;
}

void PhongMaterial::wireframe(bool value) {
    m_pPrivate->m_wireframe = value;
}
