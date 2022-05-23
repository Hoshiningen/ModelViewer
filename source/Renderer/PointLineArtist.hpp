#pragma once

#include "GeometryArtist.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class PointLineArtist : public GeometryArtist {
public:
    explicit PointLineArtist(Shader* pShader);
    virtual ~PointLineArtist();

    virtual void createVertexArrays() override;
    virtual bool draw(const VertexBuffered& geometry, const glm::vec4& color) override;

private:
    void drawLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) const;
    void drawPoint(const glm::vec3& pos, const glm::vec4& color) const;

    GLuint m_vbo = 0;
};
