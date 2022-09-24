#include "UI/Components/FileExplorer/FileSelector.hpp"
#include "UI/Components/FileExplorer/FileExplorer.hpp"
#include "UI/Components/FileExplorer/DirectoryView.hpp"
#include "UI/Utility.hpp"

#include <algorithm>
#include <ranges>

#include <imgui.h>

void FileSelector::onFileSelected(const std::filesystem::path& path) {

    m_selectedPath = path;
    m_selectedFileBuffer = {};
    std::ranges::copy(m_selectedPath.filename().string(), m_selectedFileBuffer.begin());
}

void FileSelector::render() {

    if (ImGui::Begin(windowId())) {

        const float inputWidth = ImGui::GetContentRegionAvail().x - (200.f + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x * 2);
        ImGui::PushItemWidth(inputWidth);

        ImGui::InputTextWithHint("##SelectedFile", "Selected file...", m_selectedFileBuffer.data(), m_selectedFileBuffer.size());
        ImGui::PopItemWidth();

        ImGui::SameLine();

        const float filterComboWidth = 200.f + ImGui::GetStyle().ItemSpacing.x;
        ImGui::PushItemWidth(filterComboWidth);

        if (ImGui::Combo("##Filter", &m_selectedFilter, m_filterNames.data(), m_filterNames.size()))
            filterChanged(m_dataModel.m_nameFilters.at(m_selectedFilter));

        ImGui::PopItemWidth();

        ImGui::SetCursorPosX(Utility::ComputeRightAlignedCursorPos({ 100, 100 }));
        if (ImGui::Button("Select", { 100, ImGui::GetFrameHeight() })) {
            accepted(m_selectedPath);

            ImGui::CloseCurrentPopup();
            m_selectedFileBuffer = {};
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", { 100, ImGui::GetFrameHeight() })) {
            canceled();
            
            ImGui::CloseCurrentPopup();
            m_selectedFileBuffer = {};
        }

        ImGui::End();
    }
}

void FileSelector::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const FileExplorer::DataModel*>(pFrom);
    if (!pModel || pModel->m_nameFilters.empty())
        return;

    m_dataModel.m_nameFilters = pModel->m_nameFilters;

    for (const std::string& filterName : std::views::keys(m_dataModel.m_nameFilters))
        m_filterNames.push_back(filterName.c_str());

    // Fire off a filter changed event to begin filtering directory contents.
    filterChanged(m_dataModel.m_nameFilters.front());
}

const IComponent::DataModel* FileSelector::dataModel() const {
    return &m_dataModel;
}

const char* FileSelector::windowId() const {
    return "FileSelector";
}
