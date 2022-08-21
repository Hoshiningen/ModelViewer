#include "UI/Components/ModelProps.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void ModelProps::render() {

    ImGui::LabelText("Vertices", "%d", m_metadata.vertexCount);
    ImGui::LabelText("Faces", "%d", m_metadata.faceCount);

    if (ImGui::BeginTable("", 3)) {

        ImGui::BeginDisabled();
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Colors", &m_metadata.attributes[ModelMetadata::Attribute::Color]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Indices", &m_metadata.attributes[ModelMetadata::Attribute::Index]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Normals", &m_metadata.attributes[ModelMetadata::Attribute::Normal]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Positions", &m_metadata.attributes[ModelMetadata::Attribute::Position]);
        ImGui::TableNextColumn();
        ImGui::Checkbox("Has Texels", &m_metadata.attributes[ModelMetadata::Attribute::Texel]);
        ImGui::EndDisabled();
        
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::SliderFloat("Scale", &m_scale, 0.001f, 20.f))
        scaleChanged(m_scale);

    ImGui::Spacing();

    if (ImGui::SliderAngle("Pitch", &m_pitch, -90.f, 90.f))
        pitchChanged(m_pitch);

    if (ImGui::SliderAngle("Yaw", &m_yaw))
        yawChanged(m_yaw);

    if (ImGui::SliderAngle("Roll", &m_roll))
        rollChanged(m_roll);

    ImGui::Spacing();

    constexpr float kStep = 0.1f;
    if (ImGui::InputScalar("X-offset", ImGuiDataType_Float, &m_offsets.x, &kStep))
        positionOffsetsChanged(m_offsets);

    if (ImGui::InputScalar("Y-offset", ImGuiDataType_Float, &m_offsets.y, &kStep))
        positionOffsetsChanged(m_offsets);

    if (ImGui::InputScalar("Z-offset", ImGuiDataType_Float, &m_offsets.z, &kStep))
        positionOffsetsChanged(m_offsets);

    ImGui::BeginDisabled();
    ImGui::InputFloat3("Origin", glm::value_ptr(m_origin));
    ImGui::EndDisabled();
}
