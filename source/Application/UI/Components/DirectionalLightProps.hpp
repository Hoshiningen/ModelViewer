#pragma once

#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class DirectionalLightProps : public IComponent {
public:
    virtual void render() override;

    sigslot::signal<const glm::vec3&> colorChanged;
    sigslot::signal<const glm::vec3&> directionChanged;
    sigslot::signal<float> intensityChanged;

private:
    glm::vec3 m_color;
    glm::vec3 m_direction;
    float m_pitch;
    float m_yaw;
    float m_intensity;
};