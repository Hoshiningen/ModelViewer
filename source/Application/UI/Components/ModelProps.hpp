#pragma once

#include "UI/Components/IComponent.hpp"

#include <bitset>

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class ModelProps : public IComponent {
public:
    virtual void render() override;

    sigslot::signal<const glm::vec3&> positionOffsetsChanged;
    sigslot::signal<float> scaleChanged;
    sigslot::signal<float> pitchChanged;
    sigslot::signal<float> yawChanged;
    sigslot::signal<float> rollChanged;

    struct ModelMetadata {
        enum Attribute {
            Color, Index, Normal, Position, Texel
        };

        std::array<bool, 5> attributes;
        std::uint32_t vertexCount;
        std::uint32_t faceCount;
    };

private:
    float m_scale;

    float m_pitch;
    float m_yaw;
    float m_roll;

    glm::vec3 m_offsets;
    glm::vec3 m_origin;

    ModelMetadata m_metadata;
};