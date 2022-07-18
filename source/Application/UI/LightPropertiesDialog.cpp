#include "UI/LightPropertiesDialog.hpp"

#include "Camera/Camera.hpp"

#include <fstream>

#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
glm::vec3 ComputeDirection(float pitchRad, float yawRad) {
    
    const glm::quat yawQuat = glm::angleAxis(yawRad, Camera::worldUp());
    const glm::vec3 newDir = yawQuat * glm::vec3{ 0.f, 0.f, 1.f };

    // Update the right vector based on the new direction.
    const glm::vec3 right = glm::cross(glm::normalize(newDir), Camera::worldUp());
    const glm::quat pitchQuat = glm::angleAxis(pitchRad, glm::normalize(right));

    return glm::normalize(pitchQuat * newDir);
}
} // end unnamed namespace

std::string_view LightPropertiesDialog::id() const {
    return "LightPropertiesDialog";
}

nlohmann::json LightPropertiesDialog::save() const {

    nlohmann::json json;
    json[id().data()]["pitch"] = m_pitch;
    json[id().data()]["yaw"] = m_yaw;
    json[id().data()].update(m_light.save());
    
    return json;
}

void LightPropertiesDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    if (settings.contains("pitch"))
        settings["pitch"].get_to(m_pitch);

    if (settings.contains("yaw"))
        settings["yaw"].get_to(m_yaw);

    if (settings.contains(m_light.id()))
        m_light.restore(settings[m_light.id().data()]);

    m_signalLightChanged(m_light);
}

void LightPropertiesDialog::onModelLoaded(std::forward_list<VertexBuffered>*) {
    m_signalLightChanged(m_light);
}

void LightPropertiesDialog::defineUI() {

    if (ImGui::ColorEdit3("Color", glm::value_ptr(m_light.color())))
        m_signalLightChanged(m_light);

    if (ImGui::SliderFloat("Intensity", &m_light.intensity(), 0.f, 1.f))
        m_signalLightChanged(m_light);

    if (ImGui::SliderAngle("Pitch", &m_pitch, -90.f, 90.f)) {
        m_light.direction(ComputeDirection(m_pitch, m_yaw));
        m_signalLightChanged(m_light);
    }

    if (ImGui::SliderAngle("Yaw", &m_yaw, 0.f)) {
        m_light.direction(ComputeDirection(m_pitch, m_yaw));
        m_signalLightChanged(m_light);
    }

    ImGui::BeginDisabled();
    ImGui::InputFloat3("Direction", glm::value_ptr(m_light.direction()));
    ImGui::EndDisabled();
}
