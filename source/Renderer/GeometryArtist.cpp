#pragma once

#include "GeometryArtist.hpp"

#include "Shader/Shader.hpp"

GeometryArtist::GeometryArtist(Shader* pShader)
    : m_pShader(pShader) {}

GeometryArtist::~GeometryArtist() noexcept {}

Shader* GeometryArtist::shader() const {
    return m_pShader;
}
