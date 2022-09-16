#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"
#include "UI/Components/LambertianProps.hpp"
#include "UI/Components/PhongProps.hpp"
#include "UI/Components/PhongTexturedProps.hpp"

#include <array>

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class Mesh;

class ModelProps : public IComponent {
public:
    sigslot::signal<const glm::vec3&> positionOffsetsChanged;
    sigslot::signal<float> scaleChanged;
    sigslot::signal<float> pitchChanged;
    sigslot::signal<float> yawChanged;
    sigslot::signal<float> rollChanged;
    sigslot::signal<int> materialSelected;

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

        int m_selectedMaterial;
        static const std::array<const char*, 3> m_kMaterialNames;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;
    virtual void compose(const std::initializer_list<IComponent*>& components) override;

    DataModel m_model;

    LambertianProps* m_pLambertianProps = nullptr;
    PhongProps* m_pPhongProps = nullptr;
    PhongTexturedProps* m_pPhongTexturedProps = nullptr;
};
