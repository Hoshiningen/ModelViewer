#pragma once

#include "UI/Components/IComponent.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class PhongProps : public IComponent {
public:
    virtual void render();

    sigslot::signal<const glm::vec4&> ambientColorChanged;
    sigslot::signal<const glm::vec4&> diffuseColorChanged;
    sigslot::signal<const glm::vec4&> specularColorChanged;
    sigslot::signal<float> ambientIntensityChanged;
    sigslot::signal<float> diffuseIntensityChanged;
    sigslot::signal<float> specularIntensityChanged;
    sigslot::signal<float> shininessChanged;

private:
    glm::vec4 m_ambientColor;
    glm::vec4 m_diffuseColor;
    glm::vec4 m_specularColor;

    float m_ambientIntensity;
    float m_diffuseIntensity;
    float m_specularIntensity;

    float m_shininess;
};
