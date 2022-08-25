#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class LambertianMaterial;

class LambertianProps : public IComponent {
public:
    struct Model {
        glm::vec4 m_diffuseColor;
        float m_diffuseIntensity;
    };

    virtual void render();
    virtual void syncFrom(const std::any& dataModel) override;

    sigslot::signal<const glm::vec4&> diffuseColorChanged;
    sigslot::signal<float> diffuseIntensityChanged;

private:
    Model m_model;
};
