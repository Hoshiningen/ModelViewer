#pragma once

#include "Common/Constants.hpp"
#include "UI/Components/IComponent.hpp"

#include <array>
#include <bitset>

#include <sigslot/signal.hpp>

class SceneTreeComponent : public IComponent {
public:
    enum class SceneNode : uint8_t {
        Scene, Model, Material, Lighting, Light1, Light2, Light3, Camera
    };

    sigslot::signal<SceneNode> nodeSelected;
    sigslot::signal<std::uint8_t, bool> lightStatusChanged;
    sigslot::signal<int, SceneNode> materialSelected;
    sigslot::signal<> modelRemoved;

    struct DataModel : public IComponent::DataModel {
        std::array<bool, 3> m_enabledLights;
        int m_selectedMaterial = 0;
        bool m_modelLoaded = false;
    };

private:
    virtual const char* windowId() const override;
    virtual void render() override;

    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    SceneNode selectedNode() const;
    bool selected(SceneNode sceneNode) const;
    void select(SceneNode sceneNode);

    std::bitset<8> m_selectionMask;
    DataModel m_model;
};
