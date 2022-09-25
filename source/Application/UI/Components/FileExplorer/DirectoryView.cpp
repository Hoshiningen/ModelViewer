#ifdef _WIN32
#include <Windows.h>
#include <fileapi.h>
#endif

#include "UI/Components/FileExplorer/DirectoryView.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include <imgui.h>

namespace {
std::string ConvertGlobToRegex(const std::string& globPattern) {

    using namespace std::string_literals;

    std::string temp = globPattern;
    temp = std::regex_replace(temp, std::regex{ "[.]" }, "\\."); // . -> \.
    temp = std::regex_replace(temp, std::regex{ "[*]" }, ".*"); // * -> .*
    temp = std::regex_replace(temp, std::regex{ "[?]" }, "."); // ? -> .

    temp.insert(temp.begin(), '^');
    temp.push_back('$');

    return temp;
}

auto CreateFileFilter(const std::vector<std::string>& nameFilters) {

    return [filters = nameFilters](const std::filesystem::directory_entry& entry) {

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

        std::error_code errorCode;

        // Don't filter out folders.
        if (entry.is_directory(errorCode))
            return true;

        if (!entry.exists(errorCode) || !entry.is_regular_file(errorCode) || !entry.path().has_extension())
            return false;

        if (errorCode)
            std::cerr << errorCode << " -> " << errorCode.message() << "\n";

        return std::any_of(filters.cbegin(), filters.cend(), [&entry](const std::string& filter) {
            const std::regex regex{ ConvertGlobToRegex(filter) };
            return std::regex_match(entry.path().extension().string(), regex);
        });
    };
}

std::string DateTimeString(const std::filesystem::file_time_type& writeTime) {

    const auto utcTimePoint = std::chrono::file_clock::to_utc(writeTime);
    const auto sysTimePoint = std::chrono::utc_clock::to_sys(utcTimePoint);
    const auto sysTime = std::chrono::system_clock::to_time_t(sysTimePoint);

    std::string str = std::asctime(std::localtime(&sysTime));
    str.pop_back();

    return str;
}

std::string FileSizeString(std::uintmax_t fileSize) {

    int multipleOf1024 = 0;
    for (; fileSize >= 1024.; ++multipleOf1024)
        fileSize /= 1024.0;

    fileSize = std::ceil(fileSize * 10.0) / 10.0;

    const std::string units = "BKMGTPE";
    const std::string fmtString = std::format("{} {}", fileSize, units.at(multipleOf1024));

    return multipleOf1024 == 0 ? fmtString : fmtString + "B";
}
} // end unnamed namespace

void DirectoryView::onFilterChanged(const NameFilter& nameFilter) {
    m_nameFilter = CreateFileFilter(nameFilter.second);
}

void DirectoryView::onDirectorySelected(const std::filesystem::path& path) {

    std::error_code errorCode;
    if (!std::filesystem::is_directory(path, errorCode))
        return;

    if (errorCode)
        std::cerr << errorCode << " -> " << errorCode.message() << "\n";

    m_dataModel.m_workingDirectory = path;

    m_directoryPathBuffer = {};
    std::ranges::copy(m_dataModel.m_workingDirectory.string(), m_directoryPathBuffer.begin());
    
    m_selectedPath.clear();
    fileSelected(m_selectedPath);
}

void DirectoryView::render() {

    if (ImGui::Begin(windowId())) {

        if (ImGui::ArrowButton("ChangeDirectory", ImGuiDir_Up))
            onChangeDirectory();

        ImGui::SameLine();

        const float inputWidth = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(REFRESH_ICON).x - ImGui::GetStyle().ItemSpacing.x * 2);
        const ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;

        ImGui::PushItemWidth(inputWidth);

        if (ImGui::InputText("##WorkingDirectory", m_directoryPathBuffer.data(), m_directoryPathBuffer.size(), inputFlags)) {

            std::error_code errorCode;
            if (std::filesystem::is_directory(m_directoryPathBuffer.data(), errorCode)) {
                m_dataModel.m_workingDirectory = m_directoryPathBuffer.data();
            }
            else {
                m_directoryPathBuffer = {};
                std::ranges::copy(m_dataModel.m_workingDirectory.string(), m_directoryPathBuffer.begin());
            }

            if (errorCode)
                std::cerr << errorCode << " -> " << errorCode.message() << "\n";
        }

        ImGui::PopItemWidth();

        renderFileListing();
        
        ImGui::End();
    }
}

void DirectoryView::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    if (const DataModel* pModel = dynamic_cast<const DataModel*>(pFrom)) {

        m_dataModel = *pModel;

        m_directoryPathBuffer = {};
        std::ranges::copy(m_dataModel.m_workingDirectory.string(), m_directoryPathBuffer.begin());
        m_selectedPath.clear();
    }
}

const IComponent::DataModel* DirectoryView::dataModel() const {
    return &m_dataModel;
}

const char* DirectoryView::windowId() const {
    return "DirectoryView";
}

void DirectoryView::onChangeDirectory() {

    m_dataModel.m_workingDirectory = m_directoryPathBuffer.data();

    std::error_code errorCode;
    if (!std::filesystem::is_directory(m_dataModel.m_workingDirectory, errorCode))
        return;

    if (errorCode)
        std::cerr << errorCode << " -> " << errorCode.message() << "\n";

    std::vector<std::filesystem::path> splitPath{
        m_dataModel.m_workingDirectory.begin(),
        m_dataModel.m_workingDirectory.end()
    };

    if (splitPath.size() <= 2)
        return;

    splitPath.pop_back();

    m_dataModel.m_workingDirectory.clear();
    for (const std::filesystem::path& subPath : splitPath)
        m_dataModel.m_workingDirectory /= subPath;

    m_directoryPathBuffer = {};
    std::ranges::copy(m_dataModel.m_workingDirectory.string(), m_directoryPathBuffer.begin());

    directoryChanged(m_dataModel.m_workingDirectory);
}

void DirectoryView::renderFileListing() {

    const ImGuiTableFlags tableFlags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_NoSavedSettings |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Sortable;

    if (ImGui::BeginTable("FileListing", 3, tableFlags)) {

        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Date Modified");
        ImGui::TableSetupColumn("Size");
        ImGui::TableSetupScrollFreeze(3, 1);
        ImGui::TableHeadersRow();

        std::error_code errorCode;
        const auto directoryIterator = std::filesystem::directory_iterator(
            m_dataModel.m_workingDirectory,
            std::filesystem::directory_options::skip_permission_denied,
            errorCode
        );

        for (const std::filesystem::directory_entry& entry : std::views::filter(directoryIterator, m_nameFilter)) {

            if (!entry.exists(errorCode))
                continue;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            const std::filesystem::path& entryPath = entry.path();

            const char* icon = entry.is_directory(errorCode) ? FOLDER_ICON : LINES_FILE_ICON;
            const std::string name = entryPath.filename().string();

            bool selected = entry.path() == m_selectedPath;

            const ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick;
            if (ImGui::Selectable(Utility::Label(name, icon).c_str(), selected, selectableFlags)) {
                m_selectedPath = entryPath;

                if (entry.is_regular_file(errorCode))
                    fileSelected(m_selectedPath);
                else
                    fileSelected(std::filesystem::path{});
            }

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {

                if (std::filesystem::is_directory(m_selectedPath, errorCode)) {

                    m_dataModel.m_workingDirectory = m_selectedPath;

                    m_directoryPathBuffer = {};
                    std::ranges::copy(m_dataModel.m_workingDirectory.string(), m_directoryPathBuffer.begin());

                    m_selectedPath.clear();
                    directoryChanged(m_dataModel.m_workingDirectory);
                }
            }

            ImGui::TableNextColumn();
            ImGui::Text(DateTimeString(entry.last_write_time(errorCode)).c_str());

            ImGui::TableNextColumn();
            if (entry.is_regular_file(errorCode))
                ImGui::Text(FileSizeString(entry.file_size(errorCode)).c_str());
        }

        if (errorCode)
            std::cerr << errorCode << " -> " << errorCode.message() << "\n";

        ImGui::EndTable();
    }
}
