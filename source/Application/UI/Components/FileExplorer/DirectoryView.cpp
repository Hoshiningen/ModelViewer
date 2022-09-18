#include "UI/Components/FileExplorer/DirectoryView.hpp"
#include "UI/Icons.hpp"

#include <imgui.h>

void DirectoryView::render() {

    if (ImGui::Begin(windowId())) {

        if (ImGui::ArrowButton("ChangeDirectory", ImGuiDir_Up)) {
            // CD the working directory up one.
        }

        ImGui::SameLine();

        const float inputWidth = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(REFRESH_ICON).x * 2);

        ImGui::PushItemWidth(inputWidth);
        if (ImGui::InputText("", m_dataModel.directoryPathBuffer.data(), m_dataModel.directoryPathBuffer.size())) {
            // Update the working directory based on user input
        }

        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button(REFRESH_ICON)) {
            // Refresh the directory contents in the table
        }

        static bool selected[10] = {};
        if (ImGui::BeginTable("split2", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
        {
            for (int i = 0; i < 10; i++)
            {
                char label[32];
                sprintf(label, "Item %d", i);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Selectable(label, &selected[i], ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableNextColumn();
                ImGui::Text("Some other contents");
                ImGui::TableNextColumn();
                ImGui::Text("123456");
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }
}

const char* DirectoryView::windowId() const {
    return "DirectoryView";
}
