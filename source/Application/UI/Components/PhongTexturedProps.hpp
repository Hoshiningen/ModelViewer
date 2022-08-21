#pragma once

#include "UI/Components/IComponent.hpp"

#include <array>

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class Texture;

class PhongTexturedProps : public IComponent {
public:
    virtual void render() override;

    sigslot::signal<const Texture&> diffuseMapLoaded;
    sigslot::signal<const Texture&> emissiveMapLoaded;
    sigslot::signal<const Texture&> specularMapLoaded;
    sigslot::signal<float> diffuseIntensityChanged;
    sigslot::signal<float> emissiveIntensityChanged;
    sigslot::signal<float> specularIntensityChanged;
    sigslot::signal<float> shininessChanged;

private:
    std::array<char, 256> m_diffuseMapPathBuffer;
    std::array<char, 256> m_emissiveMapPathBuffer;
    std::array<char, 256> m_specularMapPathBuffer;

    float m_diffuseIntensity;
    float m_emissiveIntensity;
    float m_specularIntensity;
    
    float m_shininess;
};