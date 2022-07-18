#pragma once

#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Material/SolidMaterial.hpp"

#include <array>
#include <forward_list>
#include <vector>

#include <glm/vec4.hpp>

#include <imgui.h>

#include <sigslot/signal.hpp>

class VertexBuffered;

class MaterialPropertiesDialog : public Dialog {
public:
    using Dialog::Dialog;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalMaterialSelected, MaterialSelected)
    DEFINE_CONNECTION(m_signalTextureLoaded, TextureLoaded)

    void onModelLoaded(std::forward_list<VertexBuffered>* model);

protected:
    virtual void defineUI() override;

private:
    void setUpLambertian(LambertianMaterial& material);
    void setUpPhong(PhongMaterial& material);
    void setUpPhongTextured(PhongTexturedMaterial& material);

    // Signals
    sigslot::signal<IMaterial*> m_signalMaterialSelected;
    sigslot::signal<const Texture&> m_signalTextureLoaded;

    int m_selectedMaterialType = 1; // Set the default material to phong
    static const std::vector<const char*> m_materialTypeNames;

    std::array<char, kTextBufferSize> m_diffusePathBuffer;
    std::array<char, kTextBufferSize> m_emissivePathBuffer;
    std::array<char, kTextBufferSize> m_specularPathBuffer;

    LambertianMaterial m_lambertianMaterial;
    PhongMaterial m_phongMaterial;
    PhongTexturedMaterial m_phongTexturedMaterial;
};
