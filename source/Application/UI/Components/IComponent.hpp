#pragma once

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
};
