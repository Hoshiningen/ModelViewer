#pragma once

#include "UI/Components/IComponent.hpp"
#include "Geometry/VertexBuffered.hpp"

#include <array>
#include <forward_list>

#include <sigslot/signal.hpp>

class ModelLoaderComponent : public IComponent {
public:
    static constexpr const char* kWindowId = "Load Model";

    virtual const char* windowId() const override;
    virtual void render() override;

    sigslot::signal<std::forward_list<VertexBuffered>> modelLoaded;

private:
    std::array<char, 256> m_pathBuffer;
};
