#include "PointLineArtist.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Shader/Shader.hpp"

#include <array>

PointLineArtist::PointLineArtist(Shader* pShader)
    : GeometryArtist(pShader) {

    const std::array<glm::vec3, 2> basisVectors{
        glm::vec3{ 1.f, 0.f, 0.f },
        glm::vec3{ 0.f, 1.f, 0.f }
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * basisVectors.size(), basisVectors.data(), GL_STATIC_DRAW);
}

PointLineArtist::~PointLineArtist() {}

 bool PointLineArtist::draw(const VertexBuffered& geometry, const glm::vec4& color) {

    if (!m_pShader)
        return false;

    const auto vertices = geometry.vertices();
    if (!vertices.has_value() || vertices->size() > 2)
        return false;

    glBindVertexArray(m_vao);

    if (vertices->size() == 2)
        drawLine(vertices->front(), vertices->back(), color);

    if (vertices->size() == 1)
        drawPoint(vertices->front(), color);

    return true;
}

void PointLineArtist::createVertexArrays() {

    glBindVertexArray(0);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
}

void PointLineArtist::drawLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) const {

    const glm::mat4 shearMatrix{
        glm::vec4{ posA, 0.f },
        glm::vec4{ posB, 0.f },
        glm::vec4{},
        glm::vec4{ 0.f, 0.f, 0.f, 1.f }
    };

    m_pShader->useProgram();
    m_pShader->set("shear", shearMatrix);
    m_pShader->set("lineColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
}

void PointLineArtist::drawPoint(const glm::vec3& pos, const glm::vec4& color) const {

    const glm::mat4 shearMatrix{
        glm::vec4{ pos, 0.f },
        glm::vec4{},
        glm::vec4{},
        glm::vec4{ 0.f, 0.f, 0.f, 1.f }
    };

    m_pShader->useProgram();
    m_pShader->set("shear", shearMatrix);
    m_pShader->set("lineColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 1);
}
