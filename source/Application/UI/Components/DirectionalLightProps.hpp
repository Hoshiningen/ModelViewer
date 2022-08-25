#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class DirectionalLight;

class DirectionalLightProps : public IComponent {
public:
    struct Model {
        bool m_enabled;
        glm::vec3 m_color;
        glm::vec3 m_direction;
        float m_pitch;
        float m_yaw;
        float m_intensity;
    };

    virtual void render() override;
    virtual void syncFrom(const std::any& dataModel) override;

    sigslot::signal<const glm::vec3&> colorChanged;
    sigslot::signal<float> pitchChanged;
    sigslot::signal<float> yawChanged;
    sigslot::signal<float> intensityChanged;

private:
    Model m_model;
};
