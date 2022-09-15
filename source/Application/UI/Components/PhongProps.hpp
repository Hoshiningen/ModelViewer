#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class PhongMaterial;

class PhongProps : public IComponent {
public:
    sigslot::signal<const glm::vec4&> ambientColorChanged;
    sigslot::signal<const glm::vec4&> diffuseColorChanged;
    sigslot::signal<const glm::vec4&> specularColorChanged;
    sigslot::signal<float> ambientIntensityChanged;
    sigslot::signal<float> diffuseIntensityChanged;
    sigslot::signal<float> specularIntensityChanged;
    sigslot::signal<float> shininessChanged;
    
    struct DataModel : public IComponent::DataModel {
        glm::vec4 m_ambientColor;
        glm::vec4 m_diffuseColor;
        glm::vec4 m_specularColor;

        float m_ambientIntensity;
        float m_diffuseIntensity;
        float m_specularIntensity;

        float m_shininess;
    };

private:
    virtual void render();
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    DataModel m_model;
};
