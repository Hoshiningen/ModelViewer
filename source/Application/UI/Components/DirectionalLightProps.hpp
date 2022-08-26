#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class DirectionalLight;

class DirectionalLightProps : public IComponent {
public:
    virtual void render() override;

    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    sigslot::signal<const glm::vec3&> colorChanged;
    sigslot::signal<float> pitchChanged;
    sigslot::signal<float> yawChanged;
    sigslot::signal<float> intensityChanged;

    struct DataModel : public IComponent::DataModel{
        bool m_enabled;
        glm::vec3 m_color;
        glm::vec3 m_direction;
        float m_pitch;
        float m_yaw;
        float m_intensity;
    };

private:
    DataModel m_model;
};
