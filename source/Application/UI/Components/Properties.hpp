#pragma once

#include "UI/Components/IComponent.hpp"

#include "Common/ClassMacros.hpp"

#include <string>

class PropertiesComponent : public IComponent {
public:
    DECLARE_SETTER_COPY(propertiesComponent, IComponent*)

private:
    virtual const char* windowId() const override;
    virtual void render() override;

    IComponent* m_pPropertiesComponent = nullptr;
};
