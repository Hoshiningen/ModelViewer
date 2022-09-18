#pragma once

#include "UI/Components/IComponent.hpp"

#include <filesystem>

#include <sigslot/signal.hpp>

class DirectoryTree : public IComponent {
public:

    struct DataModel : public IComponent::DataModel {
    };

private:
    virtual void render() override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;
};
