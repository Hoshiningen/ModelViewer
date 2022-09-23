#ifdef _WIN32
#include <Windows.h>
#include <fileapi.h>
#endif

#include "UI/Components/FileExplorer/DirectoryTree.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <format>
#include <ranges>

#include <imgui.h>

namespace {
std::vector<std::string> GetDrives() {

    std::vector<std::string> drives;

#ifdef _WIN32
    std::array<char, 50> buffer{};
    const DWORD result = GetLogicalDriveStrings(static_cast<DWORD>(buffer.size()), buffer.data());

    if (result > 0 && result <= buffer.size()) {

        char* drive = buffer.data();
        while (*drive) {

            std::array<char, MAX_PATH> nameBuffer{};
            const BOOL queryResult = GetVolumeInformation(drive, nameBuffer.data(), nameBuffer.size(), nullptr, nullptr, nullptr, nullptr, 0);
            
            if (queryResult && GetDriveType(drive) == DRIVE_FIXED)
                drives.push_back(std::format("{} ({})", nameBuffer.data(), drive));

            drive += std::strlen(drive) + 1;
        }
    }
#endif
    return drives;
}

bool DirectoryFilter(const std::filesystem::directory_entry& entry) {

    if (!std::filesystem::is_directory(entry.path()))
        return false;

#ifdef _WIN32
    const DWORD attributes = GetFileAttributes(entry.path().string().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    // Only hide system and hidden files if they're not drives.
    if (entry.path().root_path() != entry.path()) {
        if (attributes & FILE_ATTRIBUTE_HIDDEN)
            return false;

        if (attributes & FILE_ATTRIBUTE_SYSTEM)
            return false;
    }
#endif
 
    return true;
}
} // end unnamed namespace

void DirectoryTree::render() {

    if (ImGui::Begin(windowId())) {

        for (const std::string& driveName : GetDrives()) {
            const std::size_t beginPos = driveName.find_first_of('(');
            const std::size_t endPos = driveName.find_first_of(')');

            buildTreeRecursive(Utility::Label(driveName, HARD_DRIVE_ICON).c_str(), driveName.substr(beginPos + 1, 3));
        }

        ImGui::End();
    }
}

const char* DirectoryTree::windowId() const {
    return "DirectoryTree";
}

void DirectoryTree::buildTreeRecursive(const char* label, const std::filesystem::path& path) {

    const std::string nodeLabel = std::format("{}##{}", label, path.string());
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (m_selectedPath == path)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    const auto numChildren = std::ranges::distance(
        std::filesystem::directory_iterator(path) |
        std::views::filter(DirectoryFilter)
    );

    if (numChildren == 0)
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    const bool driveNodeOpen = ImGui::TreeNodeEx(nodeLabel.c_str(), nodeFlags);
    if (ImGui::IsItemClicked()) {
        m_selectedPath = path;
        directorySelected(path);
    }

    if (driveNodeOpen) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path) | std::views::filter(DirectoryFilter)) {
                const std::string childLabel = entry.path().filename().string();
                buildTreeRecursive(Utility::Label(childLabel, FOLDER_ICON).c_str(), entry.path());
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            ImGui::TreePop();
            return;
        }

        ImGui::TreePop();
    }
}