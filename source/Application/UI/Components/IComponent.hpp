#pragma once

#include <any>

class IComponent {
public:
    virtual ~IComponent() = default;

    virtual const char* windowId() const { return ""; }
    virtual void render() = 0;

    virtual void syncFrom(const std::any& dataModel) {}
    virtual void syncTo(const std::any& dataModel) {}
};
