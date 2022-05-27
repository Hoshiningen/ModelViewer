#pragma once

#include "GeometryArtist.hpp"

class NonIndexedArtist : public GeometryArtist {
public:
    explicit NonIndexedArtist(Shader* pShader);
    virtual ~NonIndexedArtist() = default;

    virtual bool draw(const VertexBuffered& geometry, const glm::vec4& color) override;
    virtual bool validate(const VertexBuffered& geometry) const override;

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const override;
};