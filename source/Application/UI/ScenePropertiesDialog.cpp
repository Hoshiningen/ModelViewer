#include "UI/ScenePropertiesDialog.hpp"

#include <glm/gtc/type_ptr.hpp>

void ScenePropertiesDialog::onProjectionChange(int projection) {

    if (projection == Projection::ePerspective)
        m_projection = Projection::ePerspective;

    if (projection == Projection::eOrthographic)
        m_projection = Projection::eOrthographic;
}

void ScenePropertiesDialog::onWireframeModeChange(bool wireframeEnabled) {
    m_wireframe = wireframeEnabled;
}

void ScenePropertiesDialog::defineUI() {

    if (ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_clearColor)))
        m_clearColorChanged(m_clearColor);

    ImGui::BeginDisabled();
    ImGui::RadioButton("Perspective (P)", &m_projection, Projection::ePerspective);
    ImGui::SameLine();   
    ImGui::RadioButton("Orthographic (O)", &m_projection, Projection::eOrthographic);
    ImGui::SameLine();   
    ImGui::Checkbox("Wireframe (W)", &m_wireframe);
    ImGui::EndDisabled();
}
