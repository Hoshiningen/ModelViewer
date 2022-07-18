#include "Material/SolidMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

struct SolidMaterial::Private {
    glm::vec4 color;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SolidMaterial::Private,
        color
    )
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

std::string_view SolidMaterial::id() const {
    return "SolidMaterial";
}

nlohmann::json SolidMaterial::save() const {

    nlohmann::json json;
    json[id().data()] = *m_pPrivate;

    return json;
}

void SolidMaterial::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    *m_pPrivate = settings;
}

DEFINE_GETTER_CONST_CORRECT(SolidMaterial, color, glm::vec4, m_pPrivate->color)
DEFINE_SETTER_CONSTREF(SolidMaterial, color, m_pPrivate->color)
