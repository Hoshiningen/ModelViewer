#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>

class Shader;
class VertexBuffered;

class GeometryArtist {
public:
    explicit GeometryArtist(Shader* pShader);
    virtual ~GeometryArtist() noexcept;

    virtual bool draw(const VertexBuffered& geometry, const glm::vec4& color) = 0;
    virtual bool validate(const VertexBuffered& geometry) const = 0;

    Shader* shader() const;

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const = 0;

    GLint bufferSize(GLuint bufferId) const;

    Shader* m_pShader = nullptr;
};