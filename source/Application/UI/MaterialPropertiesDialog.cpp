#include "UI/MaterialPropertiesDialog.hpp"

#include "IO/TextureLoader.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

namespace {
static constexpr ImGuiSliderFlags kSliderFlags = ImGuiSliderFlags_AlwaysClamp;
} // end unnamed namespace

const std::vector<const char*> MaterialPropertiesDialog::m_materialTypeNames{
    "Lambertian", "Phong", "Phong Textured"
};

MaterialPropertiesDialog::MaterialPropertiesDialog()
    : Dialog() { initializeUI(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title)
    : Dialog(title) { initializeUI(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, ImGuiWindowFlags flags)
    : Dialog(title, flags) { initializeUI(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags)
    : Dialog(title, position, flags) { initializeUI(); }

MaterialPropertiesDialog::MaterialPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags)
    : Dialog(title, position, size, flags) { initializeUI(); }

void MaterialPropertiesDialog::onModelLoaded(std::forward_list<VertexBuffered>*) {

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
        m_materialSelected(&m_lambertianMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
        m_materialSelected(&m_phongMaterial);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
        m_materialSelected(&m_phongTexturedMaterial);
}

void MaterialPropertiesDialog::defineUI() {

    if (ImGui::Combo("Material", &m_selectedMaterialType, m_materialTypeNames.data(), m_materialTypeNames.size())) {

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
            m_materialSelected(&m_lambertianMaterial);

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
            m_materialSelected(&m_phongMaterial);

        if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
            m_materialSelected(&m_phongTexturedMaterial);
    }

    ImGui::Separator();
    ImGui::NewLine();

    if (m_selectedMaterialType < 0)
        return;

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Lambertian") == 0)
        setUpLambertian(m_lambertianMaterial, m_lambertianModel);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong") == 0)
        setUpPhong(m_phongMaterial, m_phongModel);

    if (std::strcmp(m_materialTypeNames.at(m_selectedMaterialType), "Phong Textured") == 0)
        setUpPhongTextured(m_phongTexturedMaterial, m_phongTexturedModel);;
}

void MaterialPropertiesDialog::initializeUI() {

    m_lambertianMaterial.diffuseColor(m_lambertianModel.diffuseColor);
    m_lambertianMaterial.diffuseIntensity(m_lambertianModel.diffuseIntensity);

    m_phongMaterial.ambientColor(m_phongModel.ambientColor);
    m_phongMaterial.ambientIntensity(m_phongModel.ambientIntensity);
    m_phongMaterial.diffuseColor(m_phongModel.diffuseColor);
    m_phongMaterial.diffuseIntensity(m_phongModel.diffuseIntensity);
    m_phongMaterial.specularColor(m_phongModel.specularColor);
    m_phongMaterial.specularIntensity(m_phongModel.specularIntensity);
    m_phongMaterial.shininess(m_phongModel.shininess);

    m_phongTexturedMaterial.ambientIntensity(m_phongTexturedModel.ambientIntensity);
    m_phongTexturedMaterial.diffuseIntensity(m_phongTexturedModel.diffuseIntensity);
    m_phongTexturedMaterial.emissiveIntensity(m_phongTexturedModel.emissiveIntensity);
    m_phongTexturedMaterial.specularIntensity(m_phongTexturedModel.specularIntensity);
    m_phongTexturedMaterial.shininess(m_phongTexturedModel.shininess);
}

void MaterialPropertiesDialog::setUpLambertian(LambertianMaterial& material, LambertianModel& model) {

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(model.diffuseColor)))
        material.diffuseColor(model.diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &model.diffuseIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.diffuseIntensity(model.diffuseIntensity);
}

void MaterialPropertiesDialog::setUpPhong(PhongMaterial& material, PhongModel& model) {

    if (ImGui::ColorEdit4("Ambient Color", glm::value_ptr(model.ambientColor)))
        material.ambientColor(model.ambientColor);

    if (ImGui::SliderFloat("Ambient Intensity", &model.ambientIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.ambientIntensity(model.ambientIntensity);

    if (ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(model.diffuseColor)))
        material.diffuseColor(model.diffuseColor);

    if (ImGui::SliderFloat("Diffuse Intensity", &model.diffuseIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.diffuseIntensity(model.diffuseIntensity);
    
    if (ImGui::ColorEdit4("Specular Color", glm::value_ptr(model.specularColor)))
        material.specularColor(model.specularColor);

    if (ImGui::SliderFloat("Specular Intensity", &model.specularIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.specularIntensity(model.specularIntensity);

    if (ImGui::SliderFloat("Shininess", &model.shininess, 1.f, 256.f, "%.0f", kSliderFlags))
        material.shininess(model.shininess);
}

void MaterialPropertiesDialog::setUpPhongTextured(PhongTexturedMaterial& material, PhongTexturedModel& model) {

    if (ImGui::SliderFloat("Ambient Intensity", &model.ambientIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.ambientIntensity(model.ambientIntensity);

    ImGui::InputTextWithHint("Diffuse Map", "Texture file path...",
        model.diffusePathBuffer.data(), model.diffusePathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(model.diffusePathBuffer.data()));
    if (ImGui::Button("Load##DiffuseMap")) {
        material.diffuseMap([&model] {
            Texture texture = TextureLoader::load(model.diffusePathBuffer.data(), Texture::Target::Texture2D);
            texture.minFilter(Texture::Filter::LinearMipmapLinear);
            texture.magFilter(Texture::Filter::Linear);
            texture.mipmap(true);
            return texture;
        }());

        m_textureLoaded(*material.diffuseMap());
    }
    ImGui::EndDisabled();

    if (ImGui::SliderFloat("Diffuse Intensity", &model.diffuseIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.diffuseIntensity(model.diffuseIntensity);

    ImGui::InputTextWithHint("Emissive Map", "Texture file path...",
        model.emissivePathBuffer.data(), model.emissivePathBuffer.size());
    
    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(model.emissivePathBuffer.data()));
    if (ImGui::Button("Load##EmissiveMap")) {
        material.emissiveMap([&model] {
            Texture texture = TextureLoader::load(model.emissivePathBuffer.data(), Texture::Target::Texture2D);
            texture.minFilter(Texture::Filter::LinearMipmapLinear);
            texture.magFilter(Texture::Filter::Linear);
            texture.mipmap(true);
            return texture;
        }());

        m_textureLoaded(*material.emissiveMap());
    }
    ImGui::EndDisabled();

    if (ImGui::SliderFloat("Emissive Intensity", &model.emissiveIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.emissiveIntensity(model.emissiveIntensity);

    ImGui::InputTextWithHint("Specular Map", "Texture file path...",
        model.specularPathBuffer.data(), model.specularPathBuffer.size());
    
    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(model.specularPathBuffer.data()));
    if (ImGui::Button("Load##SpecularMap")) {
        material.specularMap([&model] {
            Texture texture = TextureLoader::load(model.specularPathBuffer.data(), Texture::Target::Texture2D);
            texture.minFilter(Texture::Filter::LinearMipmapLinear);
            texture.magFilter(Texture::Filter::Linear);
            texture.mipmap(true);
            return texture;
        }());

        m_textureLoaded(*material.specularMap());
    }
    ImGui::EndDisabled();

    if (ImGui::SliderFloat("Specular Intensity", &model.specularIntensity, 0.f, 1.f, "%.3f", kSliderFlags))
        material.specularIntensity(model.specularIntensity);

    if (ImGui::SliderFloat("Shininess", &model.shininess, 1.f, 256.f, "%.0f", kSliderFlags))
        material.shininess(model.shininess);
}
