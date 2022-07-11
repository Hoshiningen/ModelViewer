#pragma once

#include "UI/Dialog.hpp"

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
    MaterialPropertiesDialog();
    MaterialPropertiesDialog(const std::string& title);
    MaterialPropertiesDialog(const std::string& title, ImGuiWindowFlags flags);
    MaterialPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags);
    MaterialPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags);

    template<typename... CallArgs>
    sigslot::connection connectMaterialSelected(CallArgs&&... args) {
        return m_materialSelected.connect(std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    sigslot::connection connectTextureLoaded(CallArgs&&... args) {
        return m_textureLoaded.connect(std::forward<CallArgs>(args)...);
    }

    void onModelLoaded(std::forward_list<VertexBuffered>* model);

protected:
    virtual void defineUI() override;
    virtual void initializeUI() override;

private:
    // Signals
    sigslot::signal<IMaterial*> m_materialSelected;
    sigslot::signal<const Texture&> m_textureLoaded;

    int m_selectedMaterialType = 1; // Set the default material to phong
    static const std::vector<const char*> m_materialTypeNames;

    LambertianMaterial m_lambertianMaterial;
    struct LambertianModel {
        glm::vec4 diffuseColor{ 1.f, 1.f, 1.f, 1.f };
        float diffuseIntensity = 1.f;
    } m_lambertianModel;

    PhongMaterial m_phongMaterial;
    struct PhongModel {
        glm::vec4 ambientColor{ 1.f, 1.f, 1.f, 1.f };
        glm::vec4 diffuseColor{ 1.f, 1.f, 1.f, 1.f };
        glm::vec4 specularColor{ 1.f, 1.f, 1.f, 1.f };
        float ambientIntensity = 1.f;
        float diffuseIntensity = 1.f;
        float specularIntensity = 1.f;
        float shininess = 1.f;
    } m_phongModel;

    PhongTexturedMaterial m_phongTexturedMaterial;
    struct PhongTexturedModel {
        std::array<char, kTextBufferSize> diffusePathBuffer;
        std::array<char, kTextBufferSize> emissivePathBuffer;
        std::array<char, kTextBufferSize> specularPathBuffer;
        float ambientIntensity = 1.f;
        float diffuseIntensity = 1.f;
        float emissiveIntensity = 1.f;
        float specularIntensity = 1.f;
        float shininess = 1.f;
    } m_phongTexturedModel;

    void setUpLambertian(LambertianMaterial& material, LambertianModel& model);
    void setUpPhong(PhongMaterial& material, PhongModel& model);
    void setUpPhongTextured(PhongTexturedMaterial& material, PhongTexturedModel& model);
};
