#include "Material/SolidMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

struct SolidMaterial::Private {
    glm::vec4 m_color;
};


SolidMaterial::SolidMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

SolidMaterial::~SolidMaterial() noexcept {}

SolidMaterial::SolidMaterial(const SolidMaterial& other) {
    *this = other;
}

SolidMaterial& SolidMaterial::operator=(const SolidMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

SolidMaterial::SolidMaterial(SolidMaterial&& other) noexcept {
    *this = std::move(other);
}

SolidMaterial& SolidMaterial::operator=(SolidMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void SolidMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    pShader->set("includeAmbient", false);
    pShader->set("includeDiffuse", false);
    pShader->set("includeSpecular", false);
}

void SolidMaterial::color(const glm::vec4& value) {
    m_pPrivate->m_color = value;
}

glm::vec4 SolidMaterial::color() const {
    return m_pPrivate->m_color;
}
