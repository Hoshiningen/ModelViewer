#include "UI/MaterialPropertiesDialog.hpp"

#include "IO/TextureLoader.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

namespace {
static constexpr ImGuiSliderFlags kSliderFlags = ImGuiSliderFlags_AlwaysClamp;

Texture LoadTexture(const std::filesystem::path& filePath) {

    return TextureLoader::load(filePath, Texture::Target::Texture2D);
}
} // end unnamed namespace

const std::vector<const char*> MaterialPropertiesDialog::m_materialTypeNames{
    "Lambertian", "Phong", "Phong Textured"
};

std::string_view MaterialPropertiesDialog::id() const {
    return "MaterialPropertiesDialog";
}

nlohmann::json MaterialPropertiesDialog::save() const {

    nlohmann::json json;

    json[id().data()]["selectedMaterial"] = m_materialTypeNames.at(m_selectedMaterialType);
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
        m_selectedMaterialType = std::distance(
            m_materialTypeNames.cbegin(),
            std::ranges::find(m_materialTypeNames, selectedMaterial)
        );
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

void MaterialPropertiesDialog::onModelLoaded(std::forward_list<VertexBuffered>*) {

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
        m_signalMaterialSelected(&m_lambertianMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
        m_signalMaterialSelected(&m_phongMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
        m_signalMaterialSelected(&m_phongTexturedMaterial);
}

void MaterialPropertiesDialog::defineUI() {

    if (ImGui::Combo("Material", &m_selectedMaterialType, m_materialTypeNames.data(), m_materialTypeNames.size())) {

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
            m_signalMaterialSelected(&m_lambertianMaterial);

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
            m_signalMaterialSelected(&m_phongMaterial);

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
            m_signalMaterialSelected(&m_phongTexturedMaterial);
    }

    ImGui::Separator();
    ImGui::NewLine();

    if (m_selectedMaterialType < 0)
        return;

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
        setUpLambertian(m_lambertianMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
        setUpPhong(m_phongMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
        setUpPhongTextured(m_phongTexturedMaterial);
}

void MaterialPropertiesDialog::setUpLambertian(LambertianMaterial& material) {

    ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(material.diffuseColor()));
    ImGui::SliderFloat("Diffuse Intensity", &material.diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
}

void MaterialPropertiesDialog::setUpPhong(PhongMaterial& material) {

    ImGui::ColorEdit4("Ambient Color", glm::value_ptr(material.ambientColor()));
    ImGui::SliderFloat("Ambient Intensity", &material.ambientIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(material.diffuseColor()));
    ImGui::SliderFloat("Diffuse Intensity", &material.diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    ImGui::ColorEdit4("Specular Color", glm::value_ptr(material.specularColor()));
    ImGui::SliderFloat("Specular Intensity", &material.specularIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    ImGui::SliderFloat("Shininess", &material.shininess(), 1.f, 256.f, "%.0f", kSliderFlags);
}

void MaterialPropertiesDialog::setUpPhongTextured(PhongTexturedMaterial& material) {

    ImGui::SliderFloat("Ambient Intensity", &material.ambientIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    ImGui::InputTextWithHint("Diffuse Map", "Texture file path...",
        m_diffusePathBuffer.data(), m_diffusePathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_diffusePathBuffer.data()));
    if (ImGui::Button("Load##DiffuseMap")) {
        material.diffuseMap(LoadTexture(m_diffusePathBuffer.data()));

        m_signalTextureLoaded(*material.diffuseMap());
    }
    ImGui::EndDisabled();

    ImGui::SliderFloat("Diffuse Intensity", &material.diffuseIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);

    ImGui::InputTextWithHint("Emissive Map", "Texture file path...",
        m_emissivePathBuffer.data(), m_emissivePathBuffer.size());
    
    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_emissivePathBuffer.data()));
    if (ImGui::Button("Load##EmissiveMap")) {
        material.emissiveMap(LoadTexture(m_emissivePathBuffer.data()));
        m_signalTextureLoaded(*material.emissiveMap());
    }
    ImGui::EndDisabled();

    ImGui::SliderFloat("Emissive Intensity", &material.emissiveIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);

    ImGui::InputTextWithHint("Specular Map", "Texture file path...",
        m_specularPathBuffer.data(), m_specularPathBuffer.size());
    
    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_specularPathBuffer.data()));
    if (ImGui::Button("Load##SpecularMap")) {
        material.specularMap(LoadTexture(m_specularPathBuffer.data()));
        m_signalTextureLoaded(*material.specularMap());
    }
    ImGui::EndDisabled();

    ImGui::SliderFloat("Specular Intensity", &material.specularIntensity(), 0.f, 1.f, "%.3f", kSliderFlags);
    ImGui::SliderFloat("Shininess", &material.shininess(), 1.f, 256.f, "%.0f", kSliderFlags);
}
