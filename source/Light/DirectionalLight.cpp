#include "Light/DirectionalLight.hpp"

#include "Common/Constants.hpp"
#include "Common/Math.hpp"

#include "Shader/ShaderProgram.hpp"

struct DirectionalLight::Private {
    glm::vec3 color{ 1.f };
    float intensity = 1.f;

    glm::vec3 direction{};
    float pitch = 0.f;
    float yaw = 0.f;

    bool enabled = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DirectionalLight::Private,
        color,
        intensity,
        pitch,
        yaw,
        enabled
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

    assert(index < kMaxLights);

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
    m_pPrivate->direction = RotateVector(kWorldForward, m_pPrivate->pitch, m_pPrivate->yaw, false);
}

DEFINE_GETTER_IMMUTABLE_COPY(DirectionalLight, pitch, float, m_pPrivate->pitch)
void DirectionalLight::pitch(float pitch) {

    m_pPrivate->pitch = pitch;
    m_pPrivate->direction = RotateVector(kWorldForward, m_pPrivate->pitch, m_pPrivate->yaw, false);
}

DEFINE_GETTER_IMMUTABLE_COPY(DirectionalLight, yaw, float, m_pPrivate->yaw)
void DirectionalLight::yaw(float yaw) {

    m_pPrivate->yaw = yaw;
    m_pPrivate->direction = RotateVector(kWorldForward, m_pPrivate->pitch, m_pPrivate->yaw, false);
}

DEFINE_GETTER_IMMUTABLE(DirectionalLight, direction, glm::vec3, m_pPrivate->direction)

DEFINE_GETTER_IMMUTABLE(DirectionalLight, color, glm::vec3, m_pPrivate->color)
DEFINE_SETTER_CONSTREF(DirectionalLight, color, m_pPrivate->color)

DEFINE_GETTER_IMMUTABLE_COPY(DirectionalLight, intensity, float, m_pPrivate->intensity)
DEFINE_SETTER_COPY(DirectionalLight, intensity, m_pPrivate->intensity)

DEFINE_GETTER_IMMUTABLE_COPY(DirectionalLight, enabled, bool, m_pPrivate->enabled)
DEFINE_SETTER_COPY(DirectionalLight, enabled, m_pPrivate->enabled)
