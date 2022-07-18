#include "Material/LambertianMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

struct LambertianMaterial::Private {
    glm::vec4 diffuseColor{};
    float diffuseIntensity = 0.f;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LambertianMaterial::Private,
        diffuseColor,
        diffuseIntensity
    )
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

    pShader->set("material.diffuseColor", m_pPrivate->diffuseColor);
    pShader->set("material.diffuseIntensity", m_pPrivate->diffuseIntensity);

    pShader->set("includeAmbient", false);
    pShader->set("includeDiffuse", true);
    pShader->set("includeSpecular", false);
}

std::string_view LambertianMaterial::id() const {
    return "LambertianMaterial";
}

nlohmann::json LambertianMaterial::save() const {

    nlohmann::json json;
    json[id().data()] = *m_pPrivate;

    return json;
}

void LambertianMaterial::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    *m_pPrivate = settings;
}

DEFINE_GETTER_MUTABLE(LambertianMaterial, diffuseColor, glm::vec4, m_pPrivate->diffuseColor)
DEFINE_SETTER_CONSTREF(LambertianMaterial, diffuseColor, m_pPrivate->diffuseColor)

DEFINE_GETTER_MUTABLE(LambertianMaterial, diffuseIntensity, float, m_pPrivate->diffuseIntensity)
DEFINE_SETTER_COPY(LambertianMaterial, diffuseIntensity, m_pPrivate->diffuseIntensity)
