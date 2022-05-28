#pragma once

#include "GeometryArtist.hpp"

#include "Shader/Shader.hpp"

GeometryArtist::GeometryArtist(Shader* pShader)
    : m_pShader(pShader) {}

GeometryArtist::~GeometryArtist() noexcept {}

Shader* GeometryArtist::shader() const {
    return m_pShader;
}

GLint GeometryArtist::bufferSize(GLuint bufferId) const {

    if (!glIsBuffer(bufferId))
        return 0;

    // Cache off the currently bound buffer so we can restore it later.
    GLint previousBuffer = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previousBuffer);

    GLint bufferSize = 0;
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

    // Restore the previous buffer.
    glBindBuffer(GL_ARRAY_BUFFER, previousBuffer);

    return bufferSize;
}