#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"
#include "UI/Components/TextureImporter.hpp"

#include <array>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class PhongTexturedMaterial;
class Texture;

class PhongTexturedProps : public IComponent {
public:
    PhongTexturedProps();
    virtual ~PhongTexturedProps() = default;

    sigslot::signal<const Texture&> diffuseMapLoaded;
    sigslot::signal<const Texture&> emissiveMapLoaded;
    sigslot::signal<const Texture&> specularMapLoaded;
    sigslot::signal<> diffuseMapUnloaded;
    sigslot::signal<> emissiveMapUnloaded;
    sigslot::signal<> specularMapUnloaded;
    sigslot::signal<float> diffuseIntensityChanged;
    sigslot::signal<float> emissiveIntensityChanged;
    sigslot::signal<float> specularIntensityChanged;
    sigslot::signal<float> shininessChanged;
    
    struct DataModel : public IComponent::DataModel {
        float m_diffuseIntensity = 0.f;
        float m_emissiveIntensity = 0.f;
        float m_specularIntensity = 0.f;

        float m_shininess = 0.f;

        GLuint64 m_diffuseTextureId = 0;
        GLuint64 m_emissiveTextureId = 0;
        GLuint64 m_specularTextureId = 0;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    DataModel m_model;

    TextureImporter m_diffuseTextureImporter;
    TextureImporter m_emissiveTextureImporter;
    TextureImporter m_specularTextureImporter;
};
