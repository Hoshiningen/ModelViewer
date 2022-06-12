#include "Material/MeshMaterial.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Texture/Texture.hpp"

struct MeshMaterial::Private {
    Texture m_diffuseMap;
    Texture m_specularMap;
    Texture m_emissiveMap;

    float m_specularShininess;
};

MeshMaterial::MeshMaterial()
    : m_pPrivate(std::make_unique<Private>()) {}

MeshMaterial::~MeshMaterial() noexcept {}

MeshMaterial::MeshMaterial(const MeshMaterial& other) {
    *this = other;
}

MeshMaterial& MeshMaterial::operator=(const MeshMaterial& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

MeshMaterial::MeshMaterial(MeshMaterial&& other) noexcept {
    *this = std::move(other);
}

MeshMaterial& MeshMaterial::operator=(MeshMaterial&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

void MeshMaterial::apply(ShaderProgram* pShader) const {

    if (!pShader)
        return;

    //pShader->set("Material.diffuseMap", m_pPrivate->m_diffuseMap);
}

void MeshMaterial::diffuseMap(const Texture& value) {
    m_pPrivate->m_diffuseMap = value;
}

void MeshMaterial::specularMap(const Texture& value) {
    m_pPrivate->m_specularMap = value;
}

void MeshMaterial::emissiveMap(const Texture& value) {
    m_pPrivate->m_emissiveMap = value;
}

void MeshMaterial::specularShininess(float value) {
    m_pPrivate->m_specularShininess = value;
}
