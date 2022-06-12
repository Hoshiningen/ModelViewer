#pragma once

#include "GeometryArtist.hpp"

class ShaderProgram;
class VertexBuffered;

class NonIndexedArtist : public GeometryArtist {
public:
    virtual ~NonIndexedArtist() = default;

    virtual bool draw(const VertexBuffered& geometry, class ShaderProgram* pProgram) override;
    virtual bool validate(const VertexBuffered& geometry) const override;

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const override;
};
