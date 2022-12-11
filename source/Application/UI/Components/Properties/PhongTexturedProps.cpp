#include "UI/Components/Properties/PhongTexturedProps.hpp"

#include "IO/TextureLoader.hpp"
#include "Material/PhongTexturedMaterial.hpp"
#include "Texture/Texture.hpp"
#include "UI/Components/MainFrame.hpp"

#include <filesystem>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace {
template<typename SubcomponentCallback>
void RenderTextureManipulator(ImTextureID textureId, const char* sectionLabel, TextureImporter& importer, SubcomponentCallback callback) {

    ImGui::PushID(sectionLabel);

    static_cast<IComponent&>(importer).render();

    ImGui::LabelText("", sectionLabel);

    if (ImGui::ImageButton(textureId, { 100, 100 }, { 0, 1 }, { 1, 0 })) {
        if (!ImGui::IsPopupOpen(TextureImporter::kWindowId))
            ImGui::OpenPopup(TextureImporter::kWindowId);
    }

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float inputWidth = ImGui::GetContentRegionAvail().x - (100.f + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x * 2);
    ImGui::PushItemWidth(inputWidth);

    ImGui::BeginDisabled(!textureId);
    callback();
    ImGui::EndDisabled();

    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::PopID();
}
} // end unnamed namespace

PhongTexturedProps::PhongTexturedProps() {

    m_diffuseTextureImporter.accepted.connect([this](const Texture& texture) {
        diffuseMapLoaded(texture);
        m_model.m_diffuseTextureId = texture.id();
    });
    m_diffuseTextureImporter.reset.connect([this] {
        diffuseMapUnloaded();
        m_model.m_diffuseTextureId = 0;
    });

    m_emissiveTextureImporter.accepted.connect([this](const Texture& texture) {
        emissiveMapLoaded(texture);
        m_model.m_emissiveTextureId = texture.id();
    });
    m_emissiveTextureImporter.reset.connect([this] {
        emissiveMapUnloaded();
        m_model.m_emissiveTextureId = 0;
    });

    m_specularTextureImporter.accepted.connect([this](const Texture& texture) {
        specularMapLoaded(texture);
        m_model.m_specularTextureId = texture.id();
    });
    m_specularTextureImporter.reset.connect([this] {
        specularMapUnloaded();
        m_model.m_specularTextureId = 0;
    });
}

void PhongTexturedProps::render() {

    ImTextureID diffuseTextureId = reinterpret_cast<ImTextureID>(m_model.m_diffuseTextureId);
    RenderTextureManipulator(diffuseTextureId, "Diffuse", m_diffuseTextureImporter, [this] {

        if (ImGui::SliderFloat("Intensity##Diffuse", &m_model.m_diffuseIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            diffuseIntensityChanged(m_model.m_diffuseIntensity);
    });

    ImGui::Spacing();

    ImTextureID emissiveTextureId = reinterpret_cast<ImTextureID>(m_model.m_emissiveTextureId);
    RenderTextureManipulator(emissiveTextureId, "Emissive", m_emissiveTextureImporter, [this] {

        if (ImGui::SliderFloat("Intensity##Emissive", &m_model.m_emissiveIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            emissiveIntensityChanged(m_model.m_emissiveIntensity);
    });

    ImGui::Spacing();

    ImTextureID specularTextureId = reinterpret_cast<ImTextureID>(m_model.m_specularTextureId);
    RenderTextureManipulator(specularTextureId, "Specular", m_specularTextureImporter, [this] {

        if (ImGui::SliderFloat("Intensity##Specular", &m_model.m_specularIntensity, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            specularIntensityChanged(m_model.m_specularIntensity);
        
        if (ImGui::SliderFloat("Shininess##1", &m_model.m_shininess, 1.f, 256.f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
            shininessChanged(m_model.m_shininess);
    });
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

    if (pModel->m_modelPath.has_parent_path()) {
        const auto SyncTextureImporter = [pModel](Texture::Type type, TextureImporter& importer) {
            TextureImporter::DataModel model;
            model.workingDirectory = pModel->m_modelPath.parent_path();

            if (pModel->m_texturePaths.contains(type)) {
                const auto foundIter = pModel->m_texturePaths.find(type);
                const std::filesystem::path texturePath = model.workingDirectory / foundIter->second;

                model.selectedTexturePath = std::filesystem::is_regular_file(texturePath) ? texturePath : "";
            }
            else
                model.selectedTexturePath = "";

            static_cast<IComponent&>(importer).syncFrom(&model);

            std::optional<std::filesystem::path> ret;
            if (!model.selectedTexturePath.empty())
                ret.emplace(model.selectedTexturePath);

            return ret;
        };

        static TextureLoader loader;
        if (const auto path = SyncTextureImporter(Texture::Type::Diffuse, m_diffuseTextureImporter); path)
            diffuseMapLoaded(loader.load(*path, Texture::Target::Texture2D));

        if (const auto path = SyncTextureImporter(Texture::Type::Emissive, m_emissiveTextureImporter); path)
            emissiveMapLoaded(loader.load(*path, Texture::Target::Texture2D));

        if (const auto path = SyncTextureImporter(Texture::Type::Specular, m_specularTextureImporter); path)
            specularMapLoaded(loader.load(*path, Texture::Target::Texture2D));
    }

    // Update the texture image buttons with loaded textures.
    if (const auto& texture = pModel->m_pPhongTexturedMat->diffuseMap(); texture.has_value())
        m_model.m_diffuseTextureId = texture->id();
    else
        m_model.m_diffuseTextureId = 0;

    if (const auto& texture = pModel->m_pPhongTexturedMat->emissiveMap(); texture.has_value())
        m_model.m_emissiveTextureId = texture->id();
    else
        m_model.m_emissiveTextureId = 0;

    if (const auto& texture = pModel->m_pPhongTexturedMat->specularMap(); texture.has_value())
        m_model.m_specularTextureId = texture->id();
    else
        m_model.m_specularTextureId = 0;
}

const IComponent::DataModel* PhongTexturedProps::dataModel() const {
    return &m_model;
}
