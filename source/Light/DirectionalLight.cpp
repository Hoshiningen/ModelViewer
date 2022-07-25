#include "Light/DirectionalLight.hpp"

#include "Common/Constants.hpp"

#include "Shader/ShaderProgram.hpp"

struct DirectionalLight::Private {
    glm::vec3 direction{};
    glm::vec3 color{};
    float intensity = 0.f;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DirectionalLight::Private,
        direction,
        color,
        intensity
    )
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

    pShader->set("directionalLight.direction", m_pPrivate->direction);
    pShader->set("directionalLight.color", m_pPrivate->color);
    pShader->set("directionalLight.intensity", m_pPrivate->intensity);
}

void DirectionalLight::apply(ShaderProgram* pShader, std::size_t index) const {

    if (!pShader)
        return;

    assert(index > 0 && index < kMaxLights);

    pShader->set(std::format("directionalLights[{}].direction", index), m_pPrivate->direction);
    pShader->set(std::format("directionalLights[{}].color", index), m_pPrivate->color);
    pShader->set(std::format("directionalLights[{}].intensity", index), m_pPrivate->intensity);
}

std::string_view DirectionalLight::id() const {
    return "DirectionalLight";
}

nlohmann::json DirectionalLight::save() const {

    nlohmann::json json;
    json[id().data()] = *m_pPrivate;

    return json;
}

void DirectionalLight::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    *m_pPrivate = settings;
}

DEFINE_GETTER_MUTABLE(DirectionalLight, direction, glm::vec3, m_pPrivate->direction)
DEFINE_SETTER_CONSTREF(DirectionalLight, direction, m_pPrivate->direction)

DEFINE_GETTER_MUTABLE(DirectionalLight, color, glm::vec3, m_pPrivate->color)
DEFINE_SETTER_CONSTREF(DirectionalLight, color, m_pPrivate->color)

DEFINE_GETTER_MUTABLE(DirectionalLight, intensity, float, m_pPrivate->intensity)
DEFINE_SETTER_COPY(DirectionalLight, intensity, m_pPrivate->intensity)
