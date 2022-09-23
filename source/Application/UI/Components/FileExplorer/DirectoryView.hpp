#pragma once

#include "UI/Components/IComponent.hpp"
#include "UI/Components/FileExplorer/FileSelector.hpp"

#include <array>
#include <filesystem>
#include <functional>

#include <sigslot/signal.hpp>

class DirectoryView : public IComponent {
public:
    sigslot::signal<const std::filesystem::path&> fileSelected;
    sigslot::signal<const std::filesystem::path&> directoryChanged;

    void onFilterChanged(const NameFilter& nameFilter);
    void onDirectorySelected(const std::filesystem::path& path);

    struct DataModel : public IComponent::DataModel {
        std::filesystem::path m_workingDirectory;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;
    virtual const char* windowId() const override;

    void onChangeDirectory();

    void renderFileListing();

    DataModel m_dataModel;

    std::array<char, 300> m_directoryPathBuffer;
    std::filesystem::path m_selectedPath;
    std::function<bool(const std::filesystem::directory_entry&)> m_nameFilter;
};
