#include "Material/PhongMaterial.hpp"
#include "Shader/ShaderProgram.hpp"

struct PhongMaterial::Private {
    glm::vec4 ambientColor{ 1.f };
    float ambientIntensity = 0.1f;

    glm::vec4 diffuseColor{ 0.6201940774917603f, 0.5923202633857727f, 0.7107843160629272f, 1.f };
    float diffuseIntensity = 1.f;

    glm::vec4 specularColor{ 1.f };
    float specularIntensity = 1.f;

    float shininess = 190.f;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhongMaterial::Private,
        ambientColor,
        ambientIntensity,
        diffuseColor,
        diffuseIntensity,
        specularColor,
        specularIntensity,
        shininess
    )
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

    pShader->set("phongMaterial.isMapped", false);

    pShader->set("phongMaterial.diffuseIntensity", m_pPrivate->diffuseIntensity);
    pShader->set("phongMaterial.ambientIntensity", m_pPrivate->ambientIntensity);
    pShader->set("phongMaterial.specularIntensity", m_pPrivate->specularIntensity);
    pShader->set("phongMaterial.shininess", m_pPrivate->shininess);
    pShader->set("phongMaterial.ambientColor", m_pPrivate->ambientColor);
    pShader->set("phongMaterial.diffuseColor", m_pPrivate->diffuseColor);
    pShader->set("phongMaterial.specularColor", m_pPrivate->specularColor);

    pShader->set("phongMaterial.hasAmbient", true);
    pShader->set("phongMaterial.hasDiffuse", true);
    pShader->set("phongMaterial.hasSpecular", true);
}

std::string_view PhongMaterial::id() const {
    return "PhongMaterial";
}

nlohmann::json PhongMaterial::save() const {

    nlohmann::json json;
    json[id().data()] = *m_pPrivate;

    return json;
}

void PhongMaterial::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    *m_pPrivate = settings;
}

DEFINE_GETTER_MUTABLE(PhongMaterial, ambientColor, glm::vec4, m_pPrivate->ambientColor)
DEFINE_SETTER_CONSTREF(PhongMaterial, ambientColor, m_pPrivate->ambientColor)

DEFINE_GETTER_MUTABLE(PhongMaterial, diffuseColor, glm::vec4, m_pPrivate->diffuseColor)
DEFINE_SETTER_CONSTREF(PhongMaterial, diffuseColor, m_pPrivate->diffuseColor)

DEFINE_GETTER_MUTABLE(PhongMaterial, specularColor, glm::vec4, m_pPrivate->specularColor)
DEFINE_SETTER_CONSTREF(PhongMaterial, specularColor, m_pPrivate->specularColor)

DEFINE_GETTER_MUTABLE(PhongMaterial, ambientIntensity, float, m_pPrivate->ambientIntensity)
DEFINE_SETTER_COPY(PhongMaterial, ambientIntensity, m_pPrivate->ambientIntensity)

DEFINE_GETTER_MUTABLE(PhongMaterial, diffuseIntensity, float, m_pPrivate->diffuseIntensity)
DEFINE_SETTER_COPY(PhongMaterial, diffuseIntensity, m_pPrivate->diffuseIntensity)

DEFINE_GETTER_MUTABLE(PhongMaterial, specularIntensity, float, m_pPrivate->specularIntensity)
DEFINE_SETTER_COPY(PhongMaterial, specularIntensity, m_pPrivate->specularIntensity)

DEFINE_GETTER_MUTABLE(PhongMaterial, shininess, float, m_pPrivate->shininess)
DEFINE_SETTER_COPY(PhongMaterial, shininess, m_pPrivate->shininess)
