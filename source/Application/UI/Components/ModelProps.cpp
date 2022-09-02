#include "UI/Components/ModelProps.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"

#include "Object/Mesh.hpp"

#include "UI/Components/MainFrame.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

const std::array<const char*, 3> ModelProps::DataModel::m_kMaterialNames{
    "Lambertian", "Phong", "Phong Textured"
};

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

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Transform")) {
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
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Material")) {
        if (ImGui::Combo("Material##Selection", &m_model.m_selectedMaterial, m_model.m_kMaterialNames.data(), 3))
            materialSelected(m_model.m_selectedMaterial);

        switch (m_model.m_selectedMaterial) {
            case 0:
                if (m_pLambertianProps)
                    m_pLambertianProps->render();

                break;
            case 1:
                if (m_pPhongProps)
                    m_pPhongProps->render();

                break;
            case 2:
                if (m_pPhongTexturedProps)
                    m_pPhongTexturedProps->render();

                break;
            default: break;
        }
    }
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

    m_model.m_selectedMaterial = [pModel]() {
        if (dynamic_cast<LambertianMaterial*>(pModel->m_pMesh->material()))
            return 0;

        if (dynamic_cast<PhongMaterial*>(pModel->m_pMesh->material()))
            return 1;

        if (dynamic_cast<PhongTexturedMaterial*>(pModel->m_pMesh->material()))
            return 2;

        return 1;
    }();

    if (m_pLambertianProps)
        m_pLambertianProps->syncFrom(pModel);
    
    if (m_pPhongProps)
        m_pPhongProps->syncFrom(pModel);
    
    if (m_pPhongTexturedProps)
        m_pPhongTexturedProps->syncFrom(pModel);
}

const IComponent::DataModel* ModelProps::dataModel() const {
    return &m_model;
}

void ModelProps::compose(const std::initializer_list<IComponent*>& components) {

    for (IComponent* pComponent : components) {

        if (auto pProp = dynamic_cast<LambertianProps*>(pComponent); pProp)
            m_pLambertianProps = pProp;

        if (auto pProp = dynamic_cast<PhongProps*>(pComponent); pProp)
            m_pPhongProps = pProp;

        if (auto pProp = dynamic_cast<PhongTexturedProps*>(pComponent); pProp)
            m_pPhongTexturedProps = pProp;
    }
}
