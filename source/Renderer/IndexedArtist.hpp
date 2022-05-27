#pragma once

#include "GeometryArtist.hpp"

class IndexedArtist : public GeometryArtist {
public:
    explicit IndexedArtist(Shader* pShader);
    virtual ~IndexedArtist() = default;

    virtual bool draw(const VertexBuffered& geometry, const glm::vec4& color) override;
    virtual bool validate(const VertexBuffered& geometry) const override;

protected:
    virtual void initializeBufferData(const VertexBuffered& geometry) const override;
};