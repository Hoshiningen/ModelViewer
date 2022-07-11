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
} // end unnamed namespace

void ModelLoaderDialog::defineUI() {

    ImGui::InputTextWithHint("", "Model file path...", m_pathBuffer.data(), m_pathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_pathBuffer.data()));
    if (ImGui::Button("Load")) {

        m_model = LoadModel(m_pathBuffer.data());
        m_modelMetadata = parseModel(m_model);

        if (!m_model.empty())
            m_modelLoaded(&m_model);
    }
    ImGui::EndDisabled();

    ImGui::Separator();

    ImGui::LabelText("Vertices", "%d", m_modelMetadata.vertexCount);
    ImGui::LabelText("Faces", "%d", m_modelMetadata.faceCount);

    if (ImGui::BeginTable("", 3)) {

        ImGui::BeginDisabled();
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Colors", &m_modelMetadata.hasColors);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Indices", &m_modelMetadata.hasIndices);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Normals", &m_modelMetadata.hasNormals);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Positions", &m_modelMetadata.hasPositions);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Texels", &m_modelMetadata.hasTexels);
        ImGui::EndDisabled();

        ImGui::EndTable();
    }
}

ModelLoaderDialog::ModelMetadata ModelLoaderDialog::parseModel(const std::forward_list<VertexBuffered>& model) const {

    ModelMetadata metadata;
    if (model.empty())
        return metadata;

    std::size_t indexCount = 0;
    for (const VertexBuffered& buffer : m_model) {

        const auto vertices = buffer.vertices();
        const auto indices = buffer.indices();

        metadata.vertexCount += vertices->size();
        indexCount += indices->size();

        metadata.hasColors |= buffer.colors().has_value();
        metadata.hasNormals |= buffer.normals().has_value();
        metadata.hasTexels |= buffer.texels().has_value();
    }

    metadata.hasPositions = metadata.vertexCount > 0;
    metadata.hasIndices = indexCount > 0;
    metadata.faceCount = metadata.hasIndices ? indexCount / 3 : 0;

    return metadata;
}
