#include "Renderer/Renderer.hpp"

#include "GeometryArtist.hpp"
#include "IndexedArtist.hpp"
#include "NonIndexedArtist.hpp"
#include "ShaderCache.hpp"
#include "ShearArtist.hpp"

#include "Camera/Camera.hpp"

#include "Light/DirectionalLight.hpp"

#include "Material/IMaterial.hpp"
#include "Material/LambertianMaterial.hpp"
#include "Material/MeshMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/SolidMaterial.hpp"
#include "Material/SolidPointLineMaterial.hpp"

#include "Shader/ShaderProgram.hpp"

#include <iostream>
#include <forward_list>

#include <glad/glad.h>

struct Renderer::Private {
    std::unique_ptr<ShaderProgram> loadShaders(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);

    DirectionalLight m_light;
    Camera* m_pCamera = nullptr;
    ShaderCache m_shaderCache;
    std::forward_list<std::unique_ptr<GeometryArtist>> m_artists;
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
    : m_pPrivate(std::make_unique<Private>()) {

    m_pPrivate->m_light.color(kWhite);
}

Renderer::~Renderer() {}

void Renderer::setup() {

    m_pPrivate->m_shaderCache.registerProgram<SolidPointLineMaterial>(m_pPrivate->loadShaders("glsl/shear.vert", "glsl/shearSolid.frag"));
    m_pPrivate->m_shaderCache.registerProgram<LambertianMaterial>(m_pPrivate->loadShaders("glsl/phong.vert", "glsl/phong.frag"));
    m_pPrivate->m_shaderCache.registerProgram<PhongMaterial>(m_pPrivate->m_shaderCache.get<LambertianMaterial>());
    m_pPrivate->m_shaderCache.registerProgram<SolidMaterial>(m_pPrivate->m_shaderCache.get<LambertianMaterial>());

    m_pPrivate->m_artists.push_front(std::make_unique<ShearArtist>());
    m_pPrivate->m_artists.push_front(std::make_unique<NonIndexedArtist>());
    m_pPrivate->m_artists.push_front(std::make_unique<IndexedArtist>());

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

    // Chain of responsibility handlers for drawing geometry.
    for (const std::unique_ptr<GeometryArtist>& pArtist : m_pPrivate->m_artists) {

        if (!pArtist->validate(geometry))
            continue;

        pShader->use();
        pShader->set("matrices.model", glm::identity<glm::mat4>());
        pShader->set("matrices.viewProjection", m_pPrivate->m_pCamera->viewProjection());
        pShader->set("eyePoint", m_pPrivate->m_pCamera->position());

        material.apply(pShader);
        m_pPrivate->m_light.apply(pShader);

        if (pArtist->draw(geometry, pShader))
            break;
    }
}
