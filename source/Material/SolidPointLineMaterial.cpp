#include "Material/SolidPointLineMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

struct SolidPointLineMaterial::Private {
    glm::vec3 m_color;
};


SolidPointLineMaterial::SolidPointLineMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

SolidPointLineMaterial::~SolidPointLineMaterial() noexcept {}

SolidPointLineMaterial::SolidPointLineMaterial(const SolidPointLineMaterial& other) {
    *this = other;
}

SolidPointLineMaterial& SolidPointLineMaterial::operator=(const SolidPointLineMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

SolidPointLineMaterial::SolidPointLineMaterial(SolidPointLineMaterial&& other) noexcept {
    *this = std::move(other);
}

SolidPointLineMaterial& SolidPointLineMaterial::operator=(SolidPointLineMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void SolidPointLineMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    pShader->set("material.color", m_pPrivate->m_color);
}

void SolidPointLineMaterial::color(const glm::vec3& value) {
    m_pPrivate->m_color = value;
}
