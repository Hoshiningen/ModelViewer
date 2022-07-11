#include "Light/DirectionalLight.hpp"
#include "Shader/ShaderProgram.hpp"

struct DirectionalLight::Private {
    glm::vec3 m_direction;
    glm::vec3 m_color;
    float m_intensity = 0.f;
};


DirectionalLight::DirectionalLight()
    : m_pPrivate(std::make_unique<Private>()) {}

DirectionalLight::~DirectionalLight() noexcept {}

DirectionalLight::DirectionalLight(const DirectionalLight& other) {
    *this = other;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept {
    *this = std::move(other);
}

DirectionalLight& DirectionalLight::operator=(DirectionalLight&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void DirectionalLight::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;
    
    pShader->set("directionalLight.direction", m_pPrivate->m_direction);
    pShader->set("directionalLight.color", m_pPrivate->m_color);
    pShader->set("directionalLight.intensity", m_pPrivate->m_intensity);
}

void DirectionalLight::direction(const glm::vec3& value) {
    m_pPrivate->m_direction = value;
}

void DirectionalLight::color(const glm::vec3& value) {
    m_pPrivate->m_color = value;
}

void DirectionalLight::intensity(float value) {
    m_pPrivate->m_intensity = value;
}
