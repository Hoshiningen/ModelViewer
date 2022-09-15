#include "UI/Components/ModelLoader.hpp"
#include "UI/Utility.hpp"

#include "IO/GeometryLoader.hpp"

#include <filesystem>

#include <imgui.h>

namespace {
std::forward_list<VertexBuffered> LoadModel(const std::filesystem::path& filePath) {

    std::forward_list<VertexBuffered> model;
    if (!std::filesystem::is_regular_file(filePath))
        return model;
    
    static GeometryLoader loader;
    model = loader.load(filePath);

    return model;
}
} // end unnamed namespace

const char* ModelLoaderComponent::windowId() const {
    return "Open Model";
}

void ModelLoaderComponent::render() {

    if (ImGui::BeginPopupModal(kWindowId, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::InputTextWithHint("", "Model file path...", m_pathBuffer.data(), m_pathBuffer.size());

        const float posX = Utility::ComputeRightAlignedCursorPos({ "Load", "Cancel" });
        ImGui::SetCursorPosX(posX);

        ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_pathBuffer.data())); {

            if (ImGui::Button("Load")) {
                const std::filesystem::path modelPath = m_pathBuffer.data();
                modelOpened(LoadModel(modelPath), modelPath);
                ImGui::CloseCurrentPopup();

                m_pathBuffer = {};
            }

            ImGui::EndDisabled();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            m_pathBuffer = {};
        }

        ImGui::EndPopup();
    }
}
