#include "UI/Components/FileExplorer/FileExplorer.hpp"

#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <array>

#include <imgui.h>
#include <imgui_internal.h>

FileExplorer::FileExplorer() {

    m_directoryView.fileSelected.connect(&FileSelector::onFileSelected, &m_fileSelector);
    m_fileSelector.accepted.connect([this](const std::filesystem::path& path) { fileSelected(path); });
    m_fileSelector.filterChanged.connect(&DirectoryView::onFilterChanged, &m_directoryView);
}

void FileExplorer::render() {

    static ImVec2 windowSize{ 800, 450 };
    ImGui::SetNextWindowSize(windowSize);

    if (ImGui::BeginPopupModal(windowId(), nullptr, ImGuiWindowFlags_NoResize)) {

        const ImGuiDockNodeFlags dockspaceFlags =
            ImGuiDockNodeFlags_PassthruCentralNode |
            ImGuiDockNodeFlags_NoTabBar |
            ImGuiDockNodeFlags_NoSplit;

        const ImGuiID dockspaceId = ImGui::GetID("FileExplorerDockspace");
        ImGui::DockSpace(dockspaceId, { 0, 0 }, dockspaceFlags);

        static bool runOnce = true;
        if (runOnce) {
            runOnce = false;

            ImGui::DockBuilderRemoveNode(dockspaceId);
            ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags);
            ImGui::DockBuilderSetNodeSize(dockspaceId, windowSize);

            ImGuiID top;
            const ImGuiID bottom = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Down, .16f, nullptr, &top);
            ImGui::DockBuilderGetNode(bottom)->LocalFlags |= ImGuiDockNodeFlags_NoResize;

            ImGuiID topRight;
            const ImGuiID topLeft = ImGui::DockBuilderSplitNode(top, ImGuiDir_Left, .27f, nullptr, &topRight);

            ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_fileSelector).windowId(), bottom);
            ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_directoryView).windowId(), topRight);
            ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_directoryTree).windowId(), topLeft);

            ImGui::DockBuilderFinish(dockspaceId);
        }

        static_cast<IComponent&>(m_directoryTree).render();
        static_cast<IComponent&>(m_directoryView).render();
        static_cast<IComponent&>(m_fileSelector).render();

        ImGui::EndPopup();
    }
}

void FileExplorer::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    if (const DataModel* pModel = dynamic_cast<const DataModel*>(pFrom)) {
        m_dataModel = *pModel;

        DirectoryView::DataModel dataModel;
        dataModel.m_workingDirectory = m_dataModel.m_workingDirectory;

        static_cast<IComponent&>(m_directoryView).syncFrom(&dataModel);
        static_cast<IComponent&>(m_fileSelector).syncFrom(pModel);
    }
}

const IComponent::DataModel* FileExplorer::dataModel() const {
    return &m_dataModel;
}

const char* FileExplorer::windowId() const {
    return kWindowId;
}
