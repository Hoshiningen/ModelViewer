#include "UI/ScenePropertiesDialog.hpp"

#include <glm/gtc/type_ptr.hpp>

std::string_view ScenePropertiesDialog::id() const {
    return "ScenePropertiesDialog";
}

nlohmann::json ScenePropertiesDialog::save() const {

    nlohmann::json json;
    json[id().data()]["clearColor"] = m_clearColor;
    json[id().data()]["projection"] = m_projection;
    json[id().data()]["wireframe"] = m_wireframe;

    return json;
}

void ScenePropertiesDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    if (settings.contains("clearColor"))
        settings["clearColor"].get_to(m_clearColor);

    if (settings.contains("projection"))
        settings["projection"].get_to(m_projection);

    if (settings.contains("wireframe"))
        settings["wireframe"].get_to(m_wireframe);

    m_signalClearColorChanged(m_clearColor);
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

void ScenePropertiesDialog::defineUI() {

    if (ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_clearColor)))
        m_signalClearColorChanged(m_clearColor);

    ImGui::BeginDisabled();
    ImGui::RadioButton("Perspective (P)", &m_projection, Projection::ePerspective);
    ImGui::SameLine();   
    ImGui::RadioButton("Orthographic (O)", &m_projection, Projection::eOrthographic);
    ImGui::SameLine();   
    ImGui::Checkbox("Wireframe (W)", &m_wireframe);
    ImGui::EndDisabled();
}
