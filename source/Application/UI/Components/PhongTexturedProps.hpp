#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"

#include <array>

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class PhongTexturedMaterial;
class Texture;

class PhongTexturedProps : public IComponent {
public:
    sigslot::signal<const Texture&> diffuseMapLoaded;
    sigslot::signal<const Texture&> emissiveMapLoaded;
    sigslot::signal<const Texture&> specularMapLoaded;
    sigslot::signal<float> diffuseIntensityChanged;
    sigslot::signal<float> emissiveIntensityChanged;
    sigslot::signal<float> specularIntensityChanged;
    sigslot::signal<float> shininessChanged;
    
    struct DataModel : public IComponent::DataModel {
        std::array<char, 256> m_diffuseMapPathBuffer;
        std::array<char, 256> m_emissiveMapPathBuffer;
        std::array<char, 256> m_specularMapPathBuffer;

        float m_diffuseIntensity;
        float m_emissiveIntensity;
        float m_specularIntensity;

        float m_shininess;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    DataModel m_model;
};
