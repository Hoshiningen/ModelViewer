#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <array>

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class Mesh;

class ModelProps : public IComponent {
public:
    struct ModelMetadata {
        enum Attribute {
            Color, Index, Normal, Position, Texel
        };

        std::uint32_t faceCount;
        std::uint32_t vertexCount;
        std::array<bool, 5> attributes;
    };

    struct DataModel : public IComponent::DataModel{
        float m_scale;

        float m_pitch;
        float m_yaw;
        float m_roll;

        glm::vec3 m_offsets;
        glm::vec3 m_origin;

        ModelMetadata m_metadata;
    };

    virtual void render() override;

    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    sigslot::signal<const glm::vec3&> positionOffsetsChanged;
    sigslot::signal<float> scaleChanged;
    sigslot::signal<float> pitchChanged;
    sigslot::signal<float> yawChanged;
    sigslot::signal<float> rollChanged;

private:
    DataModel m_model;
};