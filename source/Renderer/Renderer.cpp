#include "Renderer/Renderer.hpp"

#include "GeometryArtist.hpp"
#include "ShearArtist.hpp"
#include "NonIndexedArtist.hpp"

#include "Camera/Camera.hpp"
#include "Shader/Shader.hpp"

#include <iostream>
#include <forward_list>

#include <glad/glad.h>

struct Renderer::Private {
    void loadShaders();

    Camera* m_pCamera = nullptr;

    Shader m_shearShader;
    Shader m_nonIndexedShader;

    std::forward_list<std::unique_ptr<GeometryArtist>> m_artists;
};

void Renderer::Private::loadShaders() {

    m_shearShader.createProgram();
    m_nonIndexedShader.createProgram();

    const auto LoadShader = [](const std::filesystem::path& vert, const std::filesystem::path& frag, Shader& shader) {

        if (!std::filesystem::exists(vert) || !std::filesystem::exists(frag)) {
            std::cerr << "Could not locate shaders. Aborting.\n";
            std::exit(1);
        }

        if (auto shaderId = shader.loadShader(frag, GL_FRAGMENT_SHADER); shaderId.has_value())
            shader.attachShader(shaderId.value());

        if (auto shaderId = shader.loadShader(vert, GL_VERTEX_SHADER); shaderId.has_value())
            shader.attachShader(shaderId.value());

        if (!shader.compileAndLink()) {
            std::cerr << "Unable to compile shaders. Aborting.";
            std::exit(-1);
        }
    };

    LoadShader("glsl/shear.vert", "glsl/shear.frag", m_shearShader);
    LoadShader("glsl/nonIndexed.vert", "glsl/nonIndexed.frag", m_nonIndexedShader);
}


Renderer::Renderer()
    : m_pPrivate(std::make_unique<Private>()) {}

Renderer::~Renderer() {}

void Renderer::setup() {

    m_pPrivate->loadShaders();

    m_pPrivate->m_artists.push_front(std::make_unique<ShearArtist>(&m_pPrivate->m_shearShader));
    m_pPrivate->m_artists.push_front(std::make_unique<NonIndexedArtist>(&m_pPrivate->m_nonIndexedShader));

    glEnable(GL_MULTISAMPLE);
    glPointSize(7.f);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::camera(Camera* pCamera) {
    m_pPrivate->m_pCamera = pCamera;
}

void Renderer::draw(const VertexBuffered& geometry, const glm::vec4& color) const {
    
    // Chain of responsibility handlers for drawing geometry.
    for (const std::unique_ptr<GeometryArtist>& pArtist : m_pPrivate->m_artists) {

        Shader* pShader = pArtist->shader();
        pShader->useProgram();
        pShader->set("viewProjection", m_pPrivate->m_pCamera->viewProjection());
        pShader->set("eyePoint", m_pPrivate->m_pCamera->position());
        pShader->set("model", glm::identity<glm::mat4>());

        if (pArtist->draw(geometry, color))
            break;
    }
}
