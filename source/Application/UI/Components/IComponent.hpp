#pragma once

#include "Common/IRestorable.hpp"

//class IComponent : public IRestorable {
class IComponent {
public:
    virtual ~IComponent() = default;

    virtual const char* windowId() const { return ""; };
    virtual void render() = 0;
};
