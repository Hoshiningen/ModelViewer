#pragma once

#include <initializer_list>

class IComponent {
public:
    struct DataModel {
        virtual ~DataModel() = default;
    };

    virtual ~IComponent() = default;

    virtual const char* windowId() const { return ""; }
    virtual void render() = 0;

    virtual void syncFrom(const DataModel* pFrom) {}
    virtual const DataModel* dataModel() const { return nullptr; }

    virtual void watch(const IComponent* pComponent) {}

    virtual void compose(const std::initializer_list<IComponent*>& components) {}
};
