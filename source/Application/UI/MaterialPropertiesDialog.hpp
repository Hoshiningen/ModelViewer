#pragma once

#include "UI/Dialog.hpp"

#include "Common/ClassMacros.hpp"
#include "Common/SignalMacros.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"

#include "Object/Mesh.hpp"

#include <array>
#include <forward_list>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include <glm/vec4.hpp>

#include <imgui.h>

#include <sigslot/signal.hpp>

class VertexBuffered;

class MaterialPropertiesDialog : public Dialog {
public:
    MaterialPropertiesDialog();
    MaterialPropertiesDialog(const std::string& title);
    MaterialPropertiesDialog(const std::string& title, ImGuiWindowFlags flags);
    MaterialPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags);
    MaterialPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags);

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalMaterialSelectionChanged, MaterialSelectionChanged)
    DEFINE_CONNECTION(m_signalTextureLoaded, TextureLoaded)

    void mesh(Mesh* pMesh);

protected:
    virtual void defineUI() override;

private:
    void initialize();

    using MaterialVariant = std::variant<LambertianMaterial*, PhongMaterial*, PhongTexturedMaterial*>;

    void setUpMaterialUI(MaterialVariant material);
    void setUpMaterialUI(LambertianMaterial* pMaterial);
    void setUpMaterialUI(PhongMaterial* pMaterial);
    void setUpMaterialUI(PhongTexturedMaterial* pMaterial);

    MaterialVariant materialVariantByIndex(int index) const;
    IMaterial* materialByIndex(int index)  const;

    // Signals
    sigslot::signal<> m_signalMaterialSelectionChanged;
    sigslot::signal<const Texture&> m_signalTextureLoaded;

    int m_selectedMaterialIndex = 1;
    std::vector<const char*> m_materialNames;
    std::unordered_map<std::string_view, MaterialVariant> m_materials;

    std::array<char, kTextBufferSize> m_diffusePathBuffer;
    std::array<char, kTextBufferSize> m_emissivePathBuffer;
    std::array<char, kTextBufferSize> m_specularPathBuffer;

    LambertianMaterial m_lambertianMaterial;
    PhongMaterial m_phongMaterial;
    PhongTexturedMaterial m_phongTexturedMaterial;

    Mesh* m_pMesh = nullptr;
};
