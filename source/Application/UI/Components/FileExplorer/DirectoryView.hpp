#pragma once

#include "UI/Components/IComponent.hpp"

#include <array>
#include <filesystem>

#include <sigslot/signal.hpp>

class DirectoryView : public IComponent {
public:

    struct DataModel : public IComponent::DataModel {
        std::array<char, 300> directoryPathBuffer;
        std::filesystem::path m_workingDirectory;
    };

private:
    virtual void render() override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;
};
