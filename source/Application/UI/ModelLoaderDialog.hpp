#pragma once

#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"

#include "Geometry/VertexBuffered.hpp"

#include <array>
#include <forward_list>

#include <imgui.h>

#include <sigslot/signal.hpp>

class ModelLoaderDialog : public Dialog {
public:
    using Dialog::Dialog;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalModelLoaded, ModelLoaded)

protected:
    virtual void defineUI() override;

private:
    void parseModel(const std::forward_list<VertexBuffered>& model);

    sigslot::signal<std::forward_list<VertexBuffered>*> m_signalModelLoaded;

    bool m_hasColors = false;
    bool m_hasIndices = false;
    bool m_hasNormals = false;
    bool m_hasPositions = false;
    bool m_hasTexels = false;

    std::size_t m_vertexCount = 0;
    std::size_t m_faceCount = 0;

    std::array<char, kTextBufferSize> m_pathBuffer;
    std::forward_list<VertexBuffered> m_model;
};
