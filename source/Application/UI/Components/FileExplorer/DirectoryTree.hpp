#pragma once

#include "UI/Components/IComponent.hpp"

#include <filesystem>

#include <sigslot/signal.hpp>

class DirectoryTree : public IComponent {
public:

    sigslot::signal<const std::filesystem::path&> directorySelected;
    struct DataModel : public IComponent::DataModel {
    };

private:
    virtual void render() override;
    virtual const char* windowId() const override;

    void buildTreeRecursive(const char* label, const std::filesystem::path& path);

    DataModel m_dataModel;
    std::filesystem::path m_selectedPath;
};
