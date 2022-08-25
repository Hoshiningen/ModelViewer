#include "UI/MaterialPropertiesDialog.hpp"

#include "IO/TextureLoader.hpp"

#include <array>
#include <algorithm>
#include <ranges>
#include <string>
#include <variant>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

namespace {
static constexpr ImGuiSliderFlags kSliderFlags = ImGuiSliderFlags_AlwaysClamp;

Texture LoadTexture(const std::filesystem::path& filePath) {
    return TextureLoader::load(filePath, Texture::Target::Texture2D, false);
}

template<typename... Handlers>
struct Visitor : Handlers... {
    using Handlers::operator()...;
};

template<typename ...Handlers>
Visitor(Handlers...) -> Visitor<Handlers...>;

} // end unnamed namespace

MaterialPropertiesDialog::MaterialPropertiesDialog()
    : Dialog() { initialize(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title)
    : Dialog(title) { initialize(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, ImGuiWindowFlags flags)
    : Dialog(title, flags) { initialize(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags)
    : Dialog(title, position, flags) { initialize(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags)
    : Dialog(title, position, size, flags) { initialize(); }

std::string_view MaterialPropertiesDialog::id() const {
    return "MaterialPropertiesDialog";
}

nlohmann::json MaterialPropertiesDialog::save() const {

    nlohmann::json json;

    json[id().data()]["selectedMaterial"] = m_materialNames.at(m_selectedMaterialIndex);
    json[id().data()]["diffuseMapPath"] = m_diffusePathBuffer;
    json[id().data()]["emissiveMapPath"] = m_emissivePathBuffer;
    json[id().data()]["specularMapPath"] = m_specularPathBuffer;
    json[id().data()].update(m_lambertianMaterial.save());
    json[id().data()].update(m_phongMaterial.save());
    json[id().data()].update(m_phongTexturedMaterial.save());

    return json;
}

void MaterialPropertiesDialog::restore(const nlohmann::json& settings) {

    if (settings.contains("selectedMaterial")) {

        const std::string selectedMaterial = settings["selectedMaterial"].get<std::string>();
        m_selectedMaterialIndex = std::distance(
            m_materialNames.begin(),
            std::ranges::find(m_materialNames, selectedMaterial)
        );

        if (m_pMesh)
            m_pMesh->material(materialByIndex(m_selectedMaterialIndex));
    }

    if (settings.contains("diffuseMapPath"))
        settings["diffuseMapPath"].get_to(m_diffusePathBuffer);

    if (settings.contains("emissiveMapPath"))
        settings["emissiveMapPath"].get_to(m_emissivePathBuffer);

    if (settings.contains("specularMapPath"))
        settings["specularMapPath"].get_to(m_specularPathBuffer);

    if (settings.contains(m_lambertianMaterial.id()))
        m_lambertianMaterial.restore(settings[m_lambertianMaterial.id().data()]);

    if (settings.contains(m_phongMaterial.id()))
        m_phongMaterial.restore(settings[m_phongMaterial.id().data()]);

    if (settings.contains(m_phongTexturedMaterial.id()))
        m_phongTexturedMaterial.restore(settings[m_phongTexturedMaterial.id().data()]);
}

void MaterialPropertiesDialog::mesh(Mesh* pMesh) {

    m_pMesh = pMesh;

    if (m_pMesh)
        m_pMesh->material(materialByIndex(m_selectedMaterialIndex));
}

void MaterialPropertiesDialog::defineUI() {

    if (!m_pMesh)
        return;

    if (ImGui::Combo("Material", &m_selectedMaterialIndex, m_materialNames.data(), m_materialNames.size())) {

        m_pMesh->material(materialByIndex(m_selectedMaterialIndex));
        m_signalMaterialSelectionChanged();
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (m_selectedMaterialIndex >= 0)
        setUpMaterialUI(materialVariantByIndex(m_selectedMaterialIndex));
}

void MaterialPropertiesDialog::initialize() {

    m_materials[*m_materialNames.insert(m_materialNames.begin(), "Lambertian")] = &m_lambertianMaterial;
    m_materials[*m_materialNames.insert(m_materialNames.begin(), "Phong")] = &m_phongMaterial;
    m_materials[*m_materialNames.insert(m_materialNames.begin(), "Phong Textured")] = &m_phongTexturedMaterial;

    std::ranges::sort(m_materialNames);
}

void MaterialPropertiesDialog::setUpMaterialUI(MaterialVariant material) {

    std::visit(Visitor{
        [this](LambertianMaterial* pMaterial) { setUpMaterialUI(pMaterial); },
        [this](PhongMaterial* pMaterial) { setUpMaterialUI(pMaterial); },
        [this](PhongTexturedMaterial* pMaterial) { setUpMaterialUI(pMaterial); }
    }, material);
}

void MaterialPropertiesDialog::setUpMaterialUI(LambertianMaterial* pMaterial) {

    //ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(pMaterial->diffuseColor()));
    //ImGui::SliderFloat("Diffuse Intensity", &pMaterial->diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
}

void MaterialPropertiesDialog::setUpMaterialUI(PhongMaterial* pMaterial) {

    //ImGui::ColorEdit4("Ambient Color", glm::value_ptr(pMaterial->ambientColor()));
    //ImGui::SliderFloat("Ambient Intensity", &pMaterial->ambientIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(pMaterial->diffuseColor()));
    //ImGui::SliderFloat("Diffuse Intensity", &pMaterial->diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //ImGui::ColorEdit4("Specular Color", glm::value_ptr(pMaterial->specularColor()));
    //ImGui::SliderFloat("Specular Intensity", &pMaterial->specularIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //ImGui::SliderFloat("Shininess", &pMaterial->shininess(), 1.f, 256.f, "%.0f", kSliderFlags);
}

void MaterialPropertiesDialog::setUpMaterialUI(PhongTexturedMaterial* pMaterial) {

    //ImGui::InputTextWithHint("Diffuse Map", "Texture file path...",
    //    m_diffusePathBuffer.data(), m_diffusePathBuffer.size());
    //
    //ImGui::SameLine();
    //ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_diffusePathBuffer.data()));
    //if (ImGui::Button("Load##DiffuseMap")) {
    //    pMaterial->diffuseMap(LoadTexture(m_diffusePathBuffer.data()));
    //
    //    m_signalTextureLoaded(*pMaterial->diffuseMap());
    //}
    //ImGui::EndDisabled();
    //
    //ImGui::SliderFloat("Diffuse Intensity", &pMaterial->diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //
    //ImGui::InputTextWithHint("Emissive Map", "Texture file path...",
    //    m_emissivePathBuffer.data(), m_emissivePathBuffer.size());
    //
    //ImGui::SameLine();
    //ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_emissivePathBuffer.data()));
    //if (ImGui::Button("Load##EmissiveMap")) {
    //    pMaterial->emissiveMap(LoadTexture(m_emissivePathBuffer.data()));
    //    m_signalTextureLoaded(*pMaterial->emissiveMap());
    //}
    //ImGui::EndDisabled();
    //
    //ImGui::SliderFloat("Emissive Intensity", &pMaterial->emissiveIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //
    //ImGui::InputTextWithHint("Specular Map", "Texture file path...",
    //    m_specularPathBuffer.data(), m_specularPathBuffer.size());
    //
    //ImGui::SameLine();
    //ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_specularPathBuffer.data()));
    //if (ImGui::Button("Load##SpecularMap")) {
    //    pMaterial->specularMap(LoadTexture(m_specularPathBuffer.data()));
    //    m_signalTextureLoaded(*pMaterial->specularMap());
    //}
    //ImGui::EndDisabled();
    //
    //ImGui::SliderFloat("Specular Intensity", &pMaterial->specularIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    //ImGui::SliderFloat("Shininess", &pMaterial->shininess(), 1.f, 256.f, "%.0f", kSliderFlags);
}

MaterialPropertiesDialog::MaterialVariant MaterialPropertiesDialog::materialVariantByIndex(int index) const {

    if (index < 0 || index > m_materialNames.size() - 1)
        return static_cast<LambertianMaterial*>(nullptr);

    return m_materials.at(m_materialNames.at(index));
}

IMaterial* MaterialPropertiesDialog::materialByIndex(int index) const {

    return std::visit(Visitor{
        [](LambertianMaterial* pMaterial) -> IMaterial* { return pMaterial; },
        [](PhongMaterial* pMaterial) -> IMaterial* { return pMaterial; },
        [](PhongTexturedMaterial* pMaterial) -> IMaterial* { return pMaterial; }
    }, materialVariantByIndex(m_selectedMaterialIndex));
}
