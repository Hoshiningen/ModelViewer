#pragma once

#include "GeometryArtist.hpp"

class ShaderProgram;
class VertexBuffered;

class IndexedArtist : public GeometryArtist {
public:
    virtual ~IndexedArtist() = default;

    virtual bool draw(const VertexBuffered& geometry, ShaderProgram* pProgram) override;
    virtual bool validate(const VertexBuffered& geometry) const override;

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const override;
};