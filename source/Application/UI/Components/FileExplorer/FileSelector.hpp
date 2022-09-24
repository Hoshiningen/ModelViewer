#pragma once

#include "UI/Components/IComponent.hpp"

#include <array>
#include <filesystem>

#include <sigslot/signal.hpp>

using NameFilter = std::pair<std::string, std::vector<std::string>>;

class FileSelector : public IComponent {
public:
    sigslot::signal<const std::filesystem::path&> accepted;
    sigslot::signal<> canceled;
    sigslot::signal<const NameFilter&> filterChanged;

    void onFileSelected(const std::filesystem::path& path);

    struct DataModel : public IComponent::DataModel {
        std::vector<NameFilter> m_nameFilters;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;
    std::array<char, 300> m_selectedFileBuffer;
    std::filesystem::path m_selectedPath;

    std::vector<const char*> m_filterNames;
    int m_selectedFilter = 0;
};
