#pragma once

#include "GeometryArtist.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ShaderProgram;
class VertexBuffered;

class ShearArtist : public GeometryArtist {
public:
    ShearArtist();
    virtual ~ShearArtist() = default;

    virtual bool draw(const VertexBuffered& geometry, ShaderProgram* pProgram) override;
    virtual bool validate(const VertexBuffered& geometry) const override;

private:
    void drawLine(const glm::vec3& posA, const glm::vec3& posB, ShaderProgram* pProgram) const;
    void drawPoint(const glm::vec3& pos, ShaderProgram* pProgram) const;

    GLuint m_vbo = 0;
    GLuint m_vao = 0;
};
