#include "UI/Components/Properties/PhongTexturedProps.hpp"

#include "IO/TextureLoader.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Texture/Texture.hpp"
#include "UI/Components/MainFrame.hpp"

#include <filesystem>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace {
Texture LoadTexture(const std::filesystem::path& filePath) {
    return TextureLoader::load(filePath, Texture::Target::Texture2D, false);
}
} // end unnamed namespace

void PhongTexturedProps::render() {

    ImGui::InputTextWithHint("Diffuse Map", "Texture file path...",
        m_model.m_diffuseMapPathBuffer.data(), m_model.m_diffuseMapPathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_model.m_diffuseMapPathBuffer.data())); {

        if (ImGui::Button("Load##DiffuseMap"))
            diffuseMapLoaded(LoadTexture(m_model.m_diffuseMapPathBuffer.data()));

        ImGui::EndDisabled();
    }

    if (ImGui::SliderFloat("Diffuse Intensity", &m_model.m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        diffuseIntensityChanged(m_model.m_diffuseIntensity);

    ImGui::InputTextWithHint("Emissive Map", "Texture file path...",
        m_model.m_emissiveMapPathBuffer.data(), m_model.m_emissiveMapPathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_model.m_emissiveMapPathBuffer.data())); {

        if (ImGui::Button("Load##EmissiveMap"))
            emissiveMapLoaded(LoadTexture(m_model.m_emissiveMapPathBuffer.data()));

        ImGui::EndDisabled();
    }

    if (ImGui::SliderFloat("Emissive Intensity", &m_model.m_emissiveIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        emissiveIntensityChanged(m_model.m_emissiveIntensity);

    ImGui::InputTextWithHint("Specular Map", "Texture file path...",
        m_model.m_specularMapPathBuffer.data(), m_model.m_specularMapPathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_model.m_specularMapPathBuffer.data())); {

        if (ImGui::Button("Load##SpecularMap"))
            specularMapLoaded(LoadTexture(m_model.m_specularMapPathBuffer.data()));

        ImGui::EndDisabled();
    }

    if (ImGui::SliderFloat("Specular Intensity", &m_model.m_specularIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        specularIntensityChanged(m_model.m_specularIntensity);

    if (ImGui::SliderFloat("Shininess", &m_model.m_shininess, 1.f, 256.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        shininessChanged(m_model.m_shininess);
}

void PhongTexturedProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel || !pModel->m_pPhongTexturedMat)
        return;

    m_model.m_diffuseIntensity = pModel->m_pPhongTexturedMat->diffuseIntensity();
    m_model.m_emissiveIntensity = pModel->m_pPhongTexturedMat->emissiveIntensity();
    m_model.m_specularIntensity = pModel->m_pPhongTexturedMat->specularIntensity();
    m_model.m_shininess= pModel->m_pPhongTexturedMat->shininess();
}

const IComponent::DataModel* PhongTexturedProps::dataModel() const {
    return &m_model;
}
