#pragma once

#include "UI/Components/IComponent.hpp"
#include "Geometry/VertexBuffered.hpp"

#include <array>
#include <filesystem>
#include <forward_list>

#include <sigslot/signal.hpp>

class ModelLoaderComponent : public IComponent {
public:
    static constexpr const char* kWindowId = "Load Model";

    sigslot::signal<std::forward_list<VertexBuffered>, std::filesystem::path> modelOpened;

private:
    virtual const char* windowId() const override;
    virtual void render() override;

    std::array<char, 256> m_pathBuffer;
};
