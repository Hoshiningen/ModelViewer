#include "UI/Components/ModelProps.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"

#include "Object/Mesh.hpp"

#include "UI/Components/MainFrame.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void ModelProps::render() {

    ImGui::LabelText("Vertices", "%d", m_model.m_metadata.vertexCount);
    ImGui::LabelText("Faces", "%d", m_model.m_metadata.faceCount);

    if (ImGui::BeginTable("", 3)) {

        ImGui::BeginDisabled();
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Colors", &m_model.m_metadata.attributes[ModelMetadata::Attribute::Color]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Indices", &m_model.m_metadata.attributes[ModelMetadata::Attribute::Index]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Normals", &m_model.m_metadata.attributes[ModelMetadata::Attribute::Normal]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Positions", &m_model.m_metadata.attributes[ModelMetadata::Attribute::Position]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Texels", &m_model.m_metadata.attributes[ModelMetadata::Attribute::Texel]);
        ImGui::EndDisabled();
        
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::SliderFloat("Scale", &m_model.m_scale, 0.001f, 20.f))
        scaleChanged(m_model.m_scale);

    ImGui::Spacing();

    if (ImGui::SliderAngle("Pitch", &m_model.m_pitch, -90.f, 90.f))
        pitchChanged(m_model.m_pitch);

    if (ImGui::SliderAngle("Yaw", &m_model.m_yaw))
        yawChanged(m_model.m_yaw);

    if (ImGui::SliderAngle("Roll", &m_model.m_roll))
        rollChanged(m_model.m_roll);

    ImGui::Spacing();

    constexpr float kStep = 0.1f;
    if (ImGui::InputScalar("X-offset", ImGuiDataType_Float, &m_model.m_offsets.x, &kStep))
        positionOffsetsChanged(m_model.m_offsets);

    if (ImGui::InputScalar("Y-offset", ImGuiDataType_Float, &m_model.m_offsets.y, &kStep))
        positionOffsetsChanged(m_model.m_offsets);

    if (ImGui::InputScalar("Z-offset", ImGuiDataType_Float, &m_model.m_offsets.z, &kStep))
        positionOffsetsChanged(m_model.m_offsets);

    ImGui::BeginDisabled();
    ImGui::InputFloat3("Origin", glm::value_ptr(m_model.m_origin));
    ImGui::EndDisabled();

    ImGui::Spacing();
}

void ModelProps::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel)
        return;

    m_model.m_scale = pModel->m_pMesh->scale();
    m_model.m_pitch = pModel->m_pMesh->pitch();
    m_model.m_yaw = pModel->m_pMesh->yaw();
    m_model.m_roll = pModel->m_pMesh->roll();
    m_model.m_offsets = pModel->m_pMesh->translate();
    m_model.m_origin = pModel->m_pMesh->position();
    m_model.m_metadata.faceCount = pModel->m_pMesh->faceCount();
    m_model.m_metadata.vertexCount = pModel->m_pMesh->vertexCount();

    m_model.m_metadata.attributes.at(ModelMetadata::Color) = pModel->m_pMesh->hasColors();
    m_model.m_metadata.attributes.at(ModelMetadata::Index) = pModel->m_pMesh->hasIndices();
    m_model.m_metadata.attributes.at(ModelMetadata::Normal) = pModel->m_pMesh->hasNormals();
    m_model.m_metadata.attributes.at(ModelMetadata::Position) = pModel->m_pMesh->hasPositions();
    m_model.m_metadata.attributes.at(ModelMetadata::Texel) = pModel->m_pMesh->hasTexels();
}

const IComponent::DataModel* ModelProps::dataModel() const {
    return &m_model;
}
