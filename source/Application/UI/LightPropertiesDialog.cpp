#include "UI/LightPropertiesDialog.hpp"

#include "Camera/Camera.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
glm::vec3 computeDirection(float pitchRad, float yawRad) {
    
    const glm::quat yawQuat = glm::angleAxis(yawRad, Camera::worldUp());
    const glm::vec3 newDir = yawQuat * glm::vec3{ 0.f, 0.f, 1.f };

    // Update the right vector based on the new direction.
    const glm::vec3 right = glm::cross(glm::normalize(newDir), Camera::worldUp());
    const glm::quat pitchQuat = glm::angleAxis(pitchRad, glm::normalize(right));

    return glm::normalize(pitchQuat * newDir);
}
} // end unnamed namespace

LightPropertiesDialog::LightPropertiesDialog()
    : Dialog() { initializeUI(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title)
    : Dialog(title) { initializeUI(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, ImGuiWindowFlags flags)
    : Dialog(title, flags) { initializeUI(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags)
    : Dialog(title, position, flags) { initializeUI(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags)
    : Dialog(title, position, size, flags) { initializeUI(); }

void LightPropertiesDialog::onModelLoaded(std::forward_list<VertexBuffered>*) {
    m_lightChanged(m_light);
}

void LightPropertiesDialog::defineUI() {

    if (ImGui::ColorEdit3("Color", glm::value_ptr(m_lightModel.color))) {
        m_light.color(m_lightModel.color);
        m_lightChanged(m_light);
    }

    if (ImGui::SliderFloat("Intensity", &m_lightModel.intensity, 0.f, 1.f)) {
        m_light.intensity(m_lightModel.intensity);
        m_lightChanged(m_light);
    }

    if (ImGui::SliderAngle("Pitch", &m_lightModel.pitch, -90.f, 90.f)) {
        m_lightModel.direction = computeDirection(m_lightModel.pitch, m_lightModel.yaw);
        m_light.direction(m_lightModel.direction);
        m_lightChanged(m_light);
    }

    if (ImGui::SliderAngle("Yaw", &m_lightModel.yaw, 0.f)) {
        m_lightModel.direction = computeDirection(m_lightModel.pitch, m_lightModel.yaw);
        m_light.direction(m_lightModel.direction);
        m_lightChanged(m_light);
    }

    ImGui::BeginDisabled();
    ImGui::InputFloat3("Direction", glm::value_ptr(m_lightModel.direction));
    ImGui::EndDisabled();
}

void LightPropertiesDialog::initializeUI() {

    m_lightModel.direction = computeDirection(m_lightModel.pitch, m_lightModel.yaw);

    m_light.color(m_lightModel.color);
    m_light.direction(m_lightModel.direction);
    m_light.intensity(m_lightModel.intensity);
}
