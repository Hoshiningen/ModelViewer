#pragma once

#include "UI/Components/IComponent.hpp"

#include <array>
#include <filesystem>

#include <sigslot/signal.hpp>

class FileSelector : public IComponent {
public:
    sigslot::signal<const std::filesystem::path&> fileSelected;
    sigslot::signal<> canceled;
    sigslot::signal<> filterChanged;

    struct DataModel : public IComponent::DataModel {
        std::array<char, 300> selectedFileBuffer;
        std::array<const char*, 2> fileFilters{ "Models (*.obj, *.flt, *.ply)", "Textures (*.jpg, *.png)" };
        int selectedFilter;
    };

private:
    virtual void render() override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;
};
