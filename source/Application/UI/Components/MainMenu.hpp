#pragma once

#include "UI/Components/IComponent.hpp"
#include "Common/ClassMacros.hpp"

#include <sigslot/signal.hpp>

class MainMenuComponent : public IComponent {
public:
    sigslot::signal<> modelClosed;
    sigslot::signal<> exited;
    sigslot::signal<> scenePropertiesOpened;
    sigslot::signal<> modelPropertiesOpened;
    sigslot::signal<> lightPropertiesOpened;

    struct DataModel : public IComponent::DataModel {
        bool m_modelLoaded = false;
    };

private:
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual void render() override;

    DataModel m_model;
};
