#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class LambertianMaterial;

class LambertianProps : public IComponent {
public:
    sigslot::signal<const glm::vec4&> diffuseColorChanged;
    sigslot::signal<float> diffuseIntensityChanged;
    
    struct DataModel : public IComponent::DataModel {
        glm::vec4 m_diffuseColor;
        float m_diffuseIntensity;
    };

private:
    virtual void render();
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    DataModel m_model;
};
