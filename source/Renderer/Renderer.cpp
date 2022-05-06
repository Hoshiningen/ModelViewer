#include "Renderer.hpp"

#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Renderer::setUp() {

    loadShaders();
    createBuffers();

    glEnable(GL_MULTISAMPLE);
    glPointSize(7.f);
}

void Renderer::canvasDimensions(const glm::ivec2& dimensions) {
    //m_orthoCamera.windowDimensions(dimensions);
    //m_perspectiveCamera.windowDimensions(dimensions);
}

void Renderer::drawLine(const glm::vec2& posA, const glm::vec2& posB, const glm::vec4& color) const {
    drawLine(glm::vec3{ posA, 0.f }, glm::vec3{ posB, 0.f }, color);
}

void Renderer::drawLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) const {

    const glm::mat4 shearMatrix{
        glm::vec4{posA, 0.f},
        glm::vec4{posB, 0.f},
        glm::vec4{},
        glm::vec4{0.f, 0.f, 0.f, 1.f}
    };

    m_shader.useProgram();  
    m_shader.set("viewProj", m_pCamera->viewProjection());
    m_shader.set("shear", shearMatrix);
    m_shader.set("lineColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawPoint(const glm::vec2& pos, const glm::vec4& color) const {
    drawPoint(glm::vec3{ pos, 0.f }, color);
}

void Renderer::drawPoint(const glm::vec3& pos, const glm::vec4& color) const {

    const glm::mat4 shearMatrix{
        glm::vec4{pos, 0.f},
        glm::vec4{},
        glm::vec4{},
        glm::vec4{0.f, 0.f, 0.f, 1.f}
    };

    m_shader.useProgram();
    m_shader.set("viewProj", m_pCamera->viewProjection());
    m_shader.set("shear", shearMatrix);
    m_shader.set("lineColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 1);
}

void Renderer::loadShaders() {

    m_shader.createProgram();
    
    const std::filesystem::path fragmentPath = "glsl/fragment.glsl";
    const std::filesystem::path vertexPath = "glsl/vertex.glsl";

    if (!std::filesystem::exists(fragmentPath) || !std::filesystem::exists(vertexPath)) {
        std::cerr << "Could not locate shaders. Aborting.\n";
        std::exit(1);
    }

    if (auto shader = m_shader.loadShader(fragmentPath, GL_FRAGMENT_SHADER); shader.has_value())
        m_shader.attachShader(shader.value());

    if (auto shader = m_shader.loadShader(vertexPath, GL_VERTEX_SHADER); shader.has_value())
        m_shader.attachShader(shader.value());

    m_shader.compileAndLink();
}

void Renderer::createBuffers() {

    glBindVertexArray(0);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    std::array<glm::vec3, 2> basisVectors {
        glm::vec3{ 1.f, 0.f, 0.f },
        glm::vec3{ 0.f, 1.f, 0.f }
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * basisVectors.size(), basisVectors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
}
