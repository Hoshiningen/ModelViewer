#pragma once

#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class LambertianProps : public IComponent {
public:
    virtual void render();

    sigslot::signal<const glm::vec4&> diffuseColorChanged;
    sigslot::signal<float> diffuseIntensityChanged;

private:
    glm::vec4 m_diffuseColor;
    float m_diffuseIntensity;
};
