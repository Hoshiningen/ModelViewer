#include "Material/LambertianMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

struct LambertianMaterial::Private {
    glm::vec4 m_diffuseColor;
    float m_diffuseIntensity = 0.f;

    bool m_wireframe = false;
};

LambertianMaterial::LambertianMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

LambertianMaterial::~LambertianMaterial() noexcept {}

LambertianMaterial::LambertianMaterial(const LambertianMaterial& other) {
    *this = other;
}

LambertianMaterial& LambertianMaterial::operator=(const LambertianMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

LambertianMaterial::LambertianMaterial(LambertianMaterial&& other) noexcept {
    *this = std::move(other);
}

LambertianMaterial& LambertianMaterial::operator=(LambertianMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void LambertianMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    pShader->set("material.diffuseColor", m_pPrivate->m_diffuseColor);
    pShader->set("material.diffuseIntensity", m_pPrivate->m_diffuseIntensity);

    pShader->set("includeAmbient", false);
    pShader->set("includeDiffuse", true);
    pShader->set("includeSpecular", false);

    pShader->set("wireframe", m_pPrivate->m_wireframe);
}

void LambertianMaterial::diffuseIntensity(float value) {
    m_pPrivate->m_diffuseIntensity = value;
}

void LambertianMaterial::diffuseColor(const glm::vec4& value) {
    m_pPrivate->m_diffuseColor = value;
}

void LambertianMaterial::wireframe(bool value) {
    m_pPrivate->m_wireframe = value;
}
