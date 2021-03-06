#include "Renderer/Renderer.hpp"

#include "ShaderCache.hpp"

#include "Camera/Camera.hpp"

#include "Common/Constants.hpp"

#include "Geometry/VertexBuffered.hpp"

#include "Light/DirectionalLight.hpp"

#include "Material/IMaterial.hpp"
#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Material/SolidMaterial.hpp"

#include "Shader/VertexAttribute.hpp"
#include "Shader/ShaderProgram.hpp"

#include "Texture/Texture.hpp"

#include <array>
#include <forward_list>
#include <iostream>
#include <set>

#include <glad/glad.h>

namespace {
std::vector<VertexAttribute> DefineAttributes(const VertexBuffered& geometry) {

    std::vector<VertexAttribute> attributes;

    // Construct attribute schematics
    if (geometry.colors())
        attributes.emplace_back("color", 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

    if (geometry.normals())
        attributes.emplace_back("normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    if (geometry.texels())
        attributes.emplace_back("texel", 2, GL_FLOAT, GL_FALSE, sizeof(glm::uvec2), 0);

    if (geometry.vertices())
        attributes.emplace_back("position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    else
        std::cerr << "Error: Unable to define vertex positions for the given geometry.\n";

    return attributes;
}

bool ConfigureAttributes(const VertexBuffered& geometry, const ShaderProgram* pProgram) {

    if (!pProgram) {
        std::cerr << "Error: Unable to configure attributes. The shader program is invalid.\n";
        return false;
    }

    std::vector<VertexAttribute> attributes = DefineAttributes(geometry);
    if (attributes.empty()) {
        std::cerr << "Error: Unable to configure attributes. The geometry has no attribute data present.";
        return false;
    }

    glBindVertexArray(geometry.id());

    // Prepare data to be uploaded to the GPU
    for (const VertexAttribute& attribute : attributes) {

        const std::string attributeName = attribute.name();
        if (!pProgram->hasAttribute(attributeName)) {
            std::cerr << "Warning: The geometry has data for the attribute \'" << attributeName << "\' but the shader doesn't support it.\n";
            continue;
        }

        const std::optional<GLuint> location = pProgram->attributeLocation(attributeName);
        const std::optional<GLuint> bufferId = geometry.attributeBufferId(attributeName);

        if (!location || !bufferId)
            return false;

        glBindBuffer(GL_ARRAY_BUFFER, *bufferId);
        glVertexAttribPointer(
            *location,
            attribute.size(),
            attribute.dataType(),
            attribute.normalized(),
            attribute.stride(),
            attribute.offset()
        );
        glEnableVertexAttribArray(*location);
    }

    glBindVertexArray(0);
    return true;
}

bool LoadBufferData(const VertexBuffered& geometry, const ShaderProgram* pProgram) {

    if (!geometry.initialized()) {
        std::cerr << "Error: Unable to load buffer data for geometry that isn't initialized.\n";
        return false;
    }

    const auto colors = geometry.colors();
    const auto indices = geometry.indices();
    const auto normals = geometry.normals();
    const auto texels = geometry.texels();
    const auto vertices = geometry.vertices();

    const auto LoadBuffer = [](GLuint bufferId, GLsizeiptr bufferSize, auto pBufferData) {
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, pBufferData, GL_STATIC_DRAW);
    };

    glBindVertexArray(geometry.id());

    if (colors && pProgram->hasAttribute("color"))
        LoadBuffer(geometry.colorBufferId(), sizeof(glm::vec4) * colors->size(), colors->data());

    if (normals && pProgram->hasAttribute("normal"))
        LoadBuffer(geometry.normalBufferId(), sizeof(glm::vec3) * normals->size(), normals->data());

    if (texels && pProgram->hasAttribute("texel"))
        LoadBuffer(geometry.texelBufferId(), sizeof(glm::uvec2) * texels->size(), texels->data());

    if (vertices && pProgram->hasAttribute("position"))
        LoadBuffer(geometry.vertexBufferId(), sizeof(glm::vec3) * vertices->size(), vertices->data());

    // Don't push up any index data if we have no vertex positions.
    if (indices && pProgram->hasAttribute("position")) {
        const GLsizeiptr bufferSize = sizeof(uint32_t) * indices->size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.indexBufferId());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, geometry.indices()->data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}
} // end unnamed namespace

struct Renderer::Private {
    std::unique_ptr<ShaderProgram> loadShaders(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);

    std::array<DirectionalLight, kMaxLights> m_lights;
    std::array<bool, kMaxLights> m_enabledLights;

    glm::vec3 m_ambientColor{};
    float m_ambientIntensity = 0.f;

    Camera* m_pCamera = nullptr;
    ShaderCache m_shaderCache;
};

std::unique_ptr<ShaderProgram> Renderer::Private::loadShaders(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {

    ShaderProgram shader;

    if (!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath)) {
        std::cerr << "Could not locate shaders. Aborting.\n";
        std::exit(1);
    }

    shader.create();

    const auto vertShader = shader.loadShader(vertexPath, GL_VERTEX_SHADER);
    const auto fragShader = shader.loadShader(fragmentPath, GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader) {
        std::cerr << "Failed to load the shaders: [" << vertexPath << ", " << fragmentPath << "]" << "\n";
        std::exit(-1);
    }
    
    shader.attachShader(*vertShader);
    shader.attachShader(*fragShader);

    if (!shader.compileAndLink()) {
        std::cerr << "Unable to compile shaders. Aborting.";
        std::exit(-1);
    }

    // Cleanup shaders since they've already been linked.
    shader.detachShader(*vertShader);
    shader.detachShader(*fragShader);
    shader.destroyShaders();

    return std::make_unique<ShaderProgram>(std::move(shader));
}


Renderer::Renderer()
    : m_pPrivate(std::make_unique<Private>()) {}

Renderer::~Renderer() {}

void Renderer::setup() {

    m_pPrivate->m_shaderCache.registerProgram<PhongTexturedMaterial>(m_pPrivate->loadShaders("glsl/phong.vert", "glsl/phong.frag"));
    m_pPrivate->m_shaderCache.registerProgram<LambertianMaterial>(m_pPrivate->loadShaders("glsl/phong.vert", "glsl/phong.frag"));
    m_pPrivate->m_shaderCache.registerProgram<PhongMaterial>(m_pPrivate->loadShaders("glsl/phong.vert", "glsl/phong.frag"));
    m_pPrivate->m_shaderCache.registerProgram<SolidMaterial>(m_pPrivate->loadShaders("glsl/phong.vert", "glsl/phong.frag"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    glPointSize(7.f);
    glLineWidth(1.f);
}

void Renderer::camera(Camera* pCamera) {
    m_pPrivate->m_pCamera = pCamera;
}

void Renderer::draw(const VertexBuffered& geometry, const IMaterial& material) const {

    ShaderProgram* pShader = m_pPrivate->m_shaderCache.get(material);
    if (!pShader) {
        assert(false);
        return;
    }

    if (!geometry.initialized()) {
        assert(false);
        return;
    }

    pShader->use();
    pShader->set("matrices.model", glm::identity<glm::mat4>());
    pShader->set("matrices.viewProjection", m_pPrivate->m_pCamera->viewProjection());
    pShader->set("eyePoint", m_pPrivate->m_pCamera->position());
    pShader->set("ambientColor", m_pPrivate->m_ambientColor);
    pShader->set("ambientIntensity", m_pPrivate->m_ambientIntensity);

    material.apply(pShader);

    for (std::size_t lightIdx = 0; lightIdx < kMaxLights; ++lightIdx) {
        pShader->set(std::format("enabledLights[{}]", lightIdx), m_pPrivate->m_enabledLights.at(lightIdx));
        m_pPrivate->m_lights.at(lightIdx).apply(pShader, lightIdx);
    }

    const auto indices = geometry.indices();
    const auto vertices = geometry.vertices();
    const VertexBuffered::PrimativeType primitive = geometry.primativeType();

    if (geometry.colors().has_value())
        pShader->set("hasVertexColor", true);

    glBindVertexArray(geometry.id());

    if (indices)
        glDrawElements(static_cast<GLenum>(primitive), indices->size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    else if (vertices)
        glDrawArrays(static_cast<GLenum>(primitive), 0, vertices->size());

    glBindVertexArray(0);
}

void Renderer::draw(const std::forward_list<VertexBuffered>& model, const IMaterial& material) const {

    for (const VertexBuffered& geometry : model)
        draw(geometry, material);
}

void Renderer::onTextureLoaded(const Texture& texture) const {

    if (!texture.initialized())
        return;

    glBindTexture(static_cast<GLenum>(texture.target()), texture.id());

    if (texture.mipmap())
        glGenerateMipmap(static_cast<GLenum>(texture.target()));

    glTexParameteri(static_cast<GLenum>(texture.target()), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(texture.minFilter()));
    glTexParameteri(static_cast<GLenum>(texture.target()), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(texture.magFilter()));
    glTexParameteri(static_cast<GLenum>(texture.target()), GL_TEXTURE_WRAP_S, static_cast<GLint>(texture.wrapS()));
    glTexParameteri(static_cast<GLenum>(texture.target()), GL_TEXTURE_WRAP_T, static_cast<GLint>(texture.wrapT()));

    if (texture.wrapS() == Texture::Wrap::ClampToBorder || texture.wrapT() == Texture::Wrap::ClampToBorder)
        glTexParameterfv(static_cast<GLenum>(texture.target()), GL_TEXTURE_BORDER_COLOR, texture.borderColor().data());

    glBindTexture(static_cast<GLenum>(texture.target()), 0);
}

void Renderer::onModelLoaded(const IMaterial* pMaterial, std::forward_list<VertexBuffered>* pModel) const {

    if (!pMaterial || !pModel)
        return;

    ShaderProgram* pProgram = m_pPrivate->m_shaderCache.get(*pMaterial);
    if (!pProgram)
        return;

    for (VertexBuffered& geometry : *pModel) {

        // Create any buffers that need to be created.
        geometry.initialize();

        if (!ConfigureAttributes(geometry, pProgram))
            return;

        if (!LoadBufferData(geometry, pProgram))
            return;
    }
}

void Renderer::onLightChanged(const DirectionalLight& light, uint8_t index, bool enabled) const {
    m_pPrivate->m_lights.at(index) = light;
    m_pPrivate->m_enabledLights.at(index) = enabled;
}

void Renderer::onAmbientColorChanged(const glm::vec3& color, float intensity) const {
    m_pPrivate->m_ambientColor = color;
    m_pPrivate->m_ambientIntensity = intensity;
}
