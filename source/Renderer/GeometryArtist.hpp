#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>

class IMaterial;
class ShaderProgram;
class VertexBuffered;

class GeometryArtist {
public:
    virtual ~GeometryArtist() noexcept = default;

    virtual bool draw(const VertexBuffered& geometry, ShaderProgram* pProgram) { return false; }
    virtual bool validate(const VertexBuffered& geometry) const { return false; }

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const {}

    GLint bufferSize(GLuint bufferId) const;
};