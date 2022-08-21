#pragma once

#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class SceneProps : public IComponent {
public:
    virtual void render() override;

    sigslot::signal<const glm::vec3&> ambientColorChanged;
    sigslot::signal<const glm::vec4&> clearColorChanged;
    sigslot::signal<float> ambientIntensityChanged;

private:
    glm::vec3 m_ambientColor;
    glm::vec4 m_clearColor;

    float m_ambientIntensity;
    bool m_wireframe;
};
