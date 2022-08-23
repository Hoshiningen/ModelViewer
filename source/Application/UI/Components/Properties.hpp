#pragma once

#include "UI/Components/IComponent.hpp"

#include "Common/ClassMacros.hpp"

#include <string>

class PropertiesComponent : public IComponent {
public:
    DECLARE_SETTER_COPY(itemProperties, IComponent*)
    DECLARE_SETTER_CONSTREF(itemName, std::string)

private:
    virtual const char* windowId() const override;
    virtual void render() override;

    IComponent* m_pItemProperties = nullptr;
    std::string m_itemName;
};
