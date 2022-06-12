#include "ShearArtist.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Shader/ShaderProgram.hpp"

#include <array>

ShearArtist::ShearArtist()
    : GeometryArtist() {

    const std::array<glm::vec3, 2> basisVectors{
        glm::vec3{ 1.f, 0.f, 0.f },
        glm::vec3{ 0.f, 1.f, 0.f }
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * basisVectors.size(), basisVectors.data(), GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

bool ShearArtist::draw(const VertexBuffered& geometry, ShaderProgram* pProgram) {

    if (!validate(geometry) || !pProgram)
        return false;

    const auto vertices = geometry.vertices();
    if (!vertices.has_value() || vertices->size() > 2)
        return false;

    if (vertices->size() == 2)
        drawLine(vertices->front(), vertices->back(), pProgram);

    if (vertices->size() == 1)
        drawPoint(vertices->front(), pProgram);

    return true;
}

bool ShearArtist::validate(const VertexBuffered& geometry) const {

    const auto vertices = geometry.vertices();
    const auto normals = geometry.normals();
    const auto indices = geometry.indices();

    if (normals.has_value() || indices.has_value() || !vertices.has_value())
        return false;

    if (vertices->size() > 2)
        return false;

    return true;
}

void ShearArtist::drawLine(const glm::vec3& posA, const glm::vec3& posB, ShaderProgram* pProgram) const {

    if (!pProgram)
        return;

    const glm::mat4 shearMatrix{
        glm::vec4{ posA, 0.f },
        glm::vec4{ posB, 0.f },
        glm::vec4{},
        glm::vec4{ 0.f, 0.f, 0.f, 1.f }
    };

    pProgram->set("matrices.shear", shearMatrix);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
}

void ShearArtist::drawPoint(const glm::vec3& pos, ShaderProgram* pProgram) const {

    if (!pProgram)
        return;

    const glm::mat4 shearMatrix{
        glm::vec4{ pos, 0.f },
        glm::vec4{},
        glm::vec4{},
        glm::vec4{ 0.f, 0.f, 0.f, 1.f }
    };

    pProgram->set("shear", shearMatrix);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 1);
}
