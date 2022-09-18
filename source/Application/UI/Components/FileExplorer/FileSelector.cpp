#include "UI/Components/FileExplorer/FileSelector.hpp"
#include "UI/Utility.hpp"

#include <imgui.h>

void FileSelector::render() {

    if (ImGui::Begin(windowId())) {

        const float inputWidth = ImGui::GetContentRegionAvail().x - (200.f + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x * 2);
    
        ImGui::PushItemWidth(inputWidth);
        ImGui::InputTextWithHint("", "Selected file...", m_dataModel.selectedFileBuffer.data(), m_dataModel.selectedFileBuffer.size());
        ImGui::PopItemWidth();

        ImGui::SameLine();

        const float filterComboWidth = 200.f + ImGui::GetStyle().ItemSpacing.x;
        ImGui::PushItemWidth(filterComboWidth);
        if (ImGui::Combo("", &m_dataModel.selectedFilter, m_dataModel.fileFilters.data(), m_dataModel.fileFilters.size()))
            filterChanged();

        ImGui::PopItemWidth();

        ImGui::SetCursorPosX(Utility::ComputeRightAlignedCursorPos({ 100, 100 }));
        if (ImGui::Button("Select", { 100, ImGui::GetFrameHeight() }))
            fileSelected("");

        ImGui::SameLine();

        if (ImGui::Button("Cancel", { 100, ImGui::GetFrameHeight() })) {
            ImGui::CloseCurrentPopup();
            m_dataModel.selectedFileBuffer = {};
            canceled();
        }

        ImGui::End();
    }
}

const char* FileSelector::windowId() const {
    return "FileSelector";
}
