#pragma once

#include "UI/Components/IComponent.hpp"

#include <bitset>

#include <sigslot/signal.hpp>

class SceneTreeComponent : public IComponent {
public:
    virtual const char* windowId() const override;
    virtual void render() override;

    enum class SceneNode : uint8_t {
        Scene, Model, Material, Lighting, Light1, Light2, Light3, Camera
    };

    sigslot::signal<SceneNode> nodeSelected;

private:
    bool selected(SceneNode sceneNode) const;
    void select(SceneNode sceneNode);

    std::bitset<8> m_selectionMask;
};
