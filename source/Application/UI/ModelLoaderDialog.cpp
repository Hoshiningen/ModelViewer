#include "ModelLoaderDialog.hpp"

#include "IO/GeometryLoader.hpp"

namespace {
std::forward_list<VertexBuffered> LoadModel(const std::filesystem::path& filePath) {

    std::forward_list<VertexBuffered> model;
    if (!std::filesystem::is_regular_file(filePath))
        return model;
    
    static GeometryLoader loader;
    model = loader.load(filePath);

    return model;
}

bool ValidFilePath(const std::filesystem::path& filePath) {

    if (filePath.empty())
        return true;

    if (!std::filesystem::is_regular_file(filePath))
        return false;

    return true;
}
} // end unnamed namespace

void ModelLoaderDialog::defineUI() {

    if (!ValidFilePath(m_pathBuffer.data())) {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        ImGui::InputTextWithHint("", "Model file path...", m_pathBuffer.data(), m_pathBuffer.size());
        ImGui::PopStyleColor();
    }
    else {
        ImGui::InputTextWithHint("", "Model file path...", m_pathBuffer.data(), m_pathBuffer.size());
    }

    ImGui::SameLine();

    if (ImGui::Button("Load")) {

        m_hasColors = false;
        m_hasNormals = false;
        m_hasPositions = false;
        m_hasTexels = false;

        m_model = LoadModel(m_pathBuffer.data());

        if (!m_model.empty())
            m_modelLoaded(&m_model);
    }

    ImGui::Separator();

    if (!m_model.empty()) {

        std::size_t vertexCount = 0;
        std::size_t indexCount = 0;

        for (const VertexBuffered& buffer : m_model) {

            const auto vertices = buffer.vertices();
            const auto indices = buffer.indices();

            vertexCount += vertices->size();
            indexCount += indices->size();

            m_hasColors |= buffer.colors().has_value();
            m_hasNormals |= buffer.normals().has_value();
            m_hasTexels |= buffer.texels().has_value();
        }

        m_hasPositions = vertexCount > 0;

        ImGui::LabelText("Vertices", "%d", vertexCount);
        ImGui::LabelText("Faces", "%d", indexCount / 3);

        if (ImGui::BeginTable("", 2)) {

            ImGui::BeginDisabled();
            ImGui::TableNextColumn();
            ImGui::Checkbox("Has Colors", &m_hasColors);
            ImGui::TableNextColumn();
            ImGui::Checkbox("Has Normals", &m_hasNormals);
            ImGui::TableNextColumn();
            ImGui::Checkbox("Has Positions", &m_hasPositions);
            ImGui::TableNextColumn();
            ImGui::Checkbox("Has Texels", &m_hasTexels);
            ImGui::EndDisabled();

            ImGui::EndTable();
        }
    }
}
