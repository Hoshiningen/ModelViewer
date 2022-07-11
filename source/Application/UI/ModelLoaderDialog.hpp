#pragma once

#include "UI/Dialog.hpp"

#include "Geometry/VertexBuffered.hpp"

#include <array>
#include <forward_list>

#include <imgui.h>

#include <sigslot/signal.hpp>

class ModelLoaderDialog : public Dialog {
public:
    using Dialog::Dialog;

    template<typename... CallArgs>
    sigslot::connection connectModelLoaded(CallArgs&&... args) {
        return m_modelLoaded.connect(std::forward<CallArgs>(args)...);
    }

protected:
    virtual void defineUI() override;

private:
    sigslot::signal<std::forward_list<VertexBuffered>*> m_modelLoaded;

    std::array<char, kTextBufferSize> m_pathBuffer;
    std::forward_list<VertexBuffered> m_model;

    struct ModelMetadata {
        bool hasColors = false;
        bool hasIndices = false;
        bool hasNormals = false;
        bool hasPositions = false;
        bool hasTexels = false;
        std::size_t vertexCount = 0;
        std::size_t faceCount = 0;
    } m_modelMetadata;

    ModelMetadata parseModel(const std::forward_list<VertexBuffered>& model) const;
};
