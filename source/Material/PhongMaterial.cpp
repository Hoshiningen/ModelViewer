#include "Material/PhongMaterial.hpp"
#include "Shader/ShaderProgram.hpp"

struct PhongMaterial::Private {
    glm::vec4 ambientColor{};
    float ambientIntensity = 0.f;

    glm::vec4 diffuseColor{};
    float diffuseIntensity = 0.f;

    glm::vec4 specularColor{};
    float specularIntensity = 0.f;

    float shininess = 0.f;

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

    pShader->set("material.diffuseIntensity", m_pPrivate->diffuseIntensity);
    pShader->set("material.ambientIntensity", m_pPrivate->ambientIntensity);
    pShader->set("material.specularIntensity", m_pPrivate->specularIntensity);
    pShader->set("material.shininess", m_pPrivate->shininess);
    pShader->set("material.ambientColor", m_pPrivate->ambientColor);
    pShader->set("material.diffuseColor", m_pPrivate->diffuseColor);
    pShader->set("material.specularColor", m_pPrivate->specularColor);

    pShader->set("includeAmbient", true);
    pShader->set("includeDiffuse", true);
    pShader->set("includeSpecular", true);
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
