#include "UI/ScenePropertiesDialog.hpp"

#include "Common/Constants.hpp"
#include "Common/Math.hpp"

#include <glm/gtc/type_ptr.hpp>

std::string_view ScenePropertiesDialog::id() const {
    return "ScenePropertiesDialog";
}

nlohmann::json ScenePropertiesDialog::save() const {

    nlohmann::json json;
    json[id().data()]["clearColor"] = m_clearColor;
    json[id().data()]["ambientColor"] = m_ambientColor;
    json[id().data()]["ambientIntensity"] = m_ambientIntensity;
    json[id().data()]["projection"] = m_projection;
    json[id().data()]["wireframe"] = m_wireframe;

    return json;
}

void ScenePropertiesDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    if (settings.contains("clearColor"))
        settings["clearColor"].get_to(m_clearColor);

    if (settings.contains("ambientColor"))
        settings["ambientColor"].get_to(m_ambientColor);

    if (settings.contains("ambientIntensity"))
        settings["ambientIntensity"].get_to(m_ambientIntensity);

    if (settings.contains("projection"))
        settings["projection"].get_to(m_projection);

    if (settings.contains("wireframe"))
        settings["wireframe"].get_to(m_wireframe);
}

glm::vec3* ScenePropertiesDialog::clearColor() {
    return &m_clearColor;
}

glm::vec3* ScenePropertiesDialog::ambientColor() {
    return &m_ambientColor;
}

float* ScenePropertiesDialog::ambientIntensity() {
    return &m_ambientIntensity;
}

void ScenePropertiesDialog::onProjectionChange(int projection) {

    if (projection == Projection::ePerspective)
        m_projection = Projection::ePerspective;

    if (projection == Projection::eOrthographic)
        m_projection = Projection::eOrthographic;
}

void ScenePropertiesDialog::onWireframeModeChange(bool wireframeEnabled) {
    m_wireframe = wireframeEnabled;
}

void ScenePropertiesDialog::mesh(Mesh* pMesh) {
    m_pMesh = pMesh;
}

void ScenePropertiesDialog::defineUI() {

    if (!m_pMesh)
        return;

    ImGui::Spacing();

    ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_clearColor));

    ImGui::Spacing();

    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientColor));
    ImGui::SliderFloat("Ambient Intensity", &m_ambientIntensity, 0.f, 1.f);
    
    ImGui::Spacing();

    ImGui::BeginDisabled();
    ImGui::RadioButton("Perspective (P)", &m_projection, Projection::ePerspective);
    ImGui::SameLine();
    ImGui::RadioButton("Orthographic (O)", &m_projection, Projection::eOrthographic);
    ImGui::EndDisabled();

    ImGui::BeginDisabled();
    ImGui::Checkbox("Wireframe (W)", &m_wireframe);
    ImGui::EndDisabled();

    ImGui::Spacing();

    if (ImGui::BeginTabBar("Properties")) {

        if (ImGui::BeginTabItem("Model")) {

            ImGui::Spacing();

            ImGui::SliderFloat("Scale", &m_pMesh->scale(), kMinScale, 20.f);
            
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::SliderAngle("Pitch", &m_pMesh->pitch(), kMinPitch, kMaxPitch);
            ImGui::SliderAngle("Yaw", &m_pMesh->yaw());
            ImGui::SliderAngle("Roll", &m_pMesh->roll());

            ImGui::Spacing();
            ImGui::Spacing();
            
            constexpr float kStep = 0.1f;
            ImGui::InputScalar("X-offset", ImGuiDataType_Float, &m_pMesh->translate().x, &kStep);
            ImGui::InputScalar("Y-offset", ImGuiDataType_Float, &m_pMesh->translate().y, &kStep);
            ImGui::InputScalar("Z-offset", ImGuiDataType_Float, &m_pMesh->translate().z, &kStep);

            ImGui::BeginDisabled();
            ImGui::InputFloat3("Origin", glm::value_ptr(m_pMesh->position()));
            ImGui::EndDisabled();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
