#include "UI/Components/TextureImporter.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include "IO/TextureLoader.hpp"

#include <algorithm>
#include <ranges>

#include <imgui.h>
#include <imgui_internal.h>

TextureImporter::TextureImporter() {

    m_fileExplorer.fileSelected.connect([this](const std::filesystem::path& selectedPath) {
        m_dataModel.selectedTexturePath = selectedPath;
        m_texturePathBuffer = {};

        std::ranges::copy(m_dataModel.selectedTexturePath.string(), m_texturePathBuffer.begin());
    });
}

void TextureImporter::render() {

    if (ImGui::BeginPopupModal(windowId(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

        static_cast<IComponent&>(m_fileExplorer).render();

        if (ImGui::Button(Utility::Icon(ELLIPSIS_ICON).c_str())) {
            if (!ImGui::IsPopupOpen(FileExplorer::kWindowId))
                ImGui::OpenPopup(FileExplorer::kWindowId);
        }

        if (ImGui::IsItemHovered()) {

            ImGui::BeginTooltip();
            ImGui::Text("Select a texture");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputTextWithHint("##File", "Texture file path...", m_texturePathBuffer.data(), m_texturePathBuffer.size());
        ImGui::EndDisabled();

        ImGui::Checkbox("Flip Vertically", &m_flipUVs);

        ImGui::Spacing();

        ImGui::SetCursorPosX(Utility::ComputeRightAlignedCursorPos({ 75, 75, 75 }));

        ImGui::BeginDisabled(m_dataModel.selectedTexturePath.empty());
        if (ImGui::Button("Reset##TextureImporter", { 75, ImGui::GetFrameHeight() })) {
            reset();

            ImGui::CloseCurrentPopup();

            m_dataModel.selectedTexturePath.clear();
            m_texturePathBuffer = {};
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(m_dataModel.selectedTexturePath.empty());
        if (ImGui::Button("Import##TextureImporter", { 75, ImGui::GetFrameHeight() })) {

            const Texture texture = TextureLoader::load(m_dataModel.selectedTexturePath, Texture::Target::Texture2D, m_flipUVs);
            accepted(texture);

            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        if (ImGui::Button("Cancel##TextureImporter", { 75, ImGui::GetFrameHeight() })) {
            canceled();

            ImGui::CloseCurrentPopup();

            m_dataModel.selectedTexturePath.clear();
            m_texturePathBuffer = {};
        }

        ImGui::EndPopup();
    }
}

void TextureImporter::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    if (auto pModel = dynamic_cast<const DataModel*>(pFrom)) {
        m_dataModel = *pModel;

        if (!m_dataModel.selectedTexturePath.empty())
            std::ranges::copy(m_dataModel.selectedTexturePath.string(), m_texturePathBuffer.begin());
        else
            m_texturePathBuffer = {};

        FileExplorer::DataModel model;
        model.m_workingDirectory = m_dataModel.workingDirectory;
        model.m_nameFilters.insert(model.m_nameFilters.cbegin(), {
            "Textures",
            {
                "*.jpg", "*.jpeg", "*.jpe", "*.jif", "*.jfif", "*.jfi",
                "*.tga", "*.icb", "*.vda", "*.vst",
                "*.png",
                "*.bmp", "*.dib",
                "*.psd",
            }
        });

        m_fileExplorer.syncFrom(&model);
    }
}

const IComponent::DataModel* TextureImporter::dataModel() const {
    return &m_dataModel;
}

const char* TextureImporter::windowId() const {
    return kWindowId;
}
