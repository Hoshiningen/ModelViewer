#pragma once

#include "UI/Components/IComponent.hpp"

#include <filesystem>

#include <sigslot/signal.hpp>

class DirectoryTree : public IComponent {
public:
    sigslot::signal<const std::filesystem::path&> directorySelected;

    void onDirectoryChanged(const std::filesystem::path& path);

private:
    virtual void render() override;
    virtual const char* windowId() const override;

    void buildTreeRecursive(const char* label, const std::filesystem::path& path);

    std::filesystem::path m_selectedPath;
};
