#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>

class Shader;
class VertexBuffered;

class GeometryArtist {
public:
    explicit GeometryArtist(Shader* pShader);
    virtual ~GeometryArtist() noexcept;

    virtual void createVertexArrays() = 0;
    virtual bool draw(const VertexBuffered& geometry, const glm::vec4& color) = 0;

    Shader* shader() const;

protected:
    GLuint m_vao = 0;
    Shader* m_pShader = nullptr;
};