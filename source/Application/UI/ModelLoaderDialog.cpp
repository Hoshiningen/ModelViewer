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

std::string_view ModelLoaderDialog::id() const {
    return "ModelLoaderDialog";
}

nlohmann::json ModelLoaderDialog::save() const {

    nlohmann::json json;
    json[id().data()]["modelPath"] = m_pathBuffer;

    return json;
}

void ModelLoaderDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    if (settings.contains("modelPath"))
        settings["modelPath"].get_to(m_pathBuffer);
}

void ModelLoaderDialog::mesh(Mesh* pMesh) {
    m_pMesh = pMesh;
}

void ModelLoaderDialog::defineUI() {

    if (!m_pMesh)
        return;

    ImGui::InputTextWithHint("", "Model file path...", m_pathBuffer.data(), m_pathBuffer.size());

    ImGui::SameLine();
    ImGui::BeginDisabled(!std::filesystem::is_regular_file(m_pathBuffer.data()));
    if (ImGui::Button("Load")) {

        m_vertexCount = 0;
        m_hasColors = false;
        m_hasIndices = false;
        m_hasNormals = false;
        m_hasPositions = false;
        m_hasTexels = false;

        m_vertexCount = 0;
        m_faceCount = 0;

        m_pMesh->model(LoadModel(m_pathBuffer.data()));
        parseModel(*m_pMesh->model());

        if (!m_pMesh->model()->empty())
            m_signalModelLoaded();
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::LabelText("Vertices", "%d", m_vertexCount);
    ImGui::LabelText("Faces", "%d", m_faceCount);

    if (ImGui::BeginTable("", 3)) {

        ImGui::BeginDisabled();
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Colors", &m_hasColors);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Indices", &m_hasIndices);
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

void  ModelLoaderDialog::parseModel(const std::forward_list<VertexBuffered>& model) {

    if (model.empty())
        return;

    std::size_t indexCount = 0;
    for (const VertexBuffered& buffer : model) {

        const auto vertices = buffer.vertices();
        const auto indices = buffer.indices();

        m_vertexCount += vertices->size();
        indexCount += indices->size();

        m_hasColors |= buffer.colors().has_value();
        m_hasNormals |= buffer.normals().has_value();
        m_hasTexels |= buffer.texels().has_value();
    }

    m_hasPositions = m_vertexCount > 0;
    m_hasIndices = indexCount > 0;
    m_faceCount = m_hasIndices ? indexCount / 3 : 0;
}
