#pragma once

#include "UI/Components/IComponent.hpp"
#include "UI/Components/FileExplorer/DirectoryTree.hpp"
#include "UI/Components/FileExplorer/DirectoryView.hpp"
#include "UI/Components/FileExplorer/FileSelector.hpp"

#include <filesystem>
#include <vector>

#include <sigslot/signal.hpp>

class FileExplorer : public IComponent {
public:
    static constexpr const char* kWindowId = "File Explorer";

    sigslot::signal<const std::filesystem::path&> fileSelected;

    FileExplorer();
    virtual ~FileExplorer() = default;

    struct DataModel : public IComponent::DataModel {
        std::filesystem::path m_workingDirectory;
        std::vector<NameFilter> m_nameFilters{{ "All Files (*.*)", {"*.*"} }};
    };

    virtual void syncFrom(const IComponent::DataModel* pFrom) override;

private:
    virtual void render() override;
    virtual const IComponent::DataModel* dataModel() const override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;

    DirectoryTree m_directoryTree;
    DirectoryView m_directoryView;
    FileSelector m_fileSelector;
};
