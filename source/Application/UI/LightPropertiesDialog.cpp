#include "UI/LightPropertiesDialog.hpp"

#include "Camera/Camera.hpp"

#include <format>
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

    for (std::size_t i = 0; i < m_lights.size(); ++i) {

        nlohmann::json& obj = json[id().data()][std::format("light{}", i + 1).c_str()];

        obj["yaw"] = m_yaws.at(i);
        obj["pitch"] = m_pitches.at(i);
        obj["enabled"] = m_enabledLights.at(i);
        obj.update(m_lights.at(i).save());
    }
    return json;
}

void LightPropertiesDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    for (std::size_t i = 0; i < kMaxLights; ++i) {

        float& pitch = m_pitches.at(i);
        float& yaw = m_yaws.at(i);
        bool& enabled = m_enabledLights.at(i);
        DirectionalLight& light = m_lights.at(i);

        const std::string lightId = std::format("light{}", i + 1);
        if (settings.contains(lightId)) {

            const nlohmann::json& obj = settings[lightId];

            if (obj.contains("pitch"))
                obj["pitch"].get_to(pitch);

            if (obj.contains("yaw"))
                obj["yaw"].get_to(yaw);

            if (obj.contains("enabled"))
                obj["enabled"].get_to(enabled);

            if (obj.contains(light.id()))
                light.restore(obj[light.id().data()]);

            light.direction(ComputeDirection(pitch, yaw));
            m_signalLightChanged(light, i, enabled);
        }
    }
}

void LightPropertiesDialog::onModelLoaded(std::forward_list<VertexBuffered>*) {

    for (std::size_t lightIdx = 0; lightIdx < kMaxLights; ++lightIdx)
        m_signalLightChanged(m_lights.at(lightIdx), lightIdx, m_enabledLights.at(lightIdx));
}

void LightPropertiesDialog::defineUI() {

    if (ImGui::BeginTable("", 3)) {

        for (std::size_t lightIdx = 0; lightIdx < kMaxLights; ++lightIdx) {

            ImGui::TableNextColumn();

            if (lightIdx == 0)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(std::format("Light {}", lightIdx + 1).c_str(), &m_enabledLights.at(lightIdx)))
                m_signalLightChanged(m_lights.at(lightIdx), lightIdx, m_enabledLights.at(lightIdx));
            
            if (lightIdx == 0)
                ImGui::EndDisabled();
        }

        ImGui::EndTable();
    }
    
    ImGui::Spacing();

    if (ImGui::BeginTabBar("DirectionalLights")) {

        for (std::size_t i = 0; i < kMaxLights; ++i) {
            
            if (!m_enabledLights.at(i))
                continue;

            const std::string lightId = std::format("Light {}", i + 1);
            if (ImGui::BeginTabItem(lightId.c_str())) {

                float& yaw = m_yaws.at(i);
                float& pitch = m_pitches.at(i);
                DirectionalLight& light = m_lights.at(i);
                const bool& enabled = m_enabledLights.at(i);

                if (ImGui::ColorEdit3("Color", glm::value_ptr(light.color())))
                    m_signalLightChanged(light, i, enabled);

                if (ImGui::SliderFloat("Intensity", &light.intensity(), 0.f, 1.f))
                    m_signalLightChanged(light, i, enabled);

                if (ImGui::SliderAngle("Pitch", &pitch, -90.f, 90.f)) {
                    light.direction(ComputeDirection(pitch, yaw));
                    m_signalLightChanged(light, i, enabled);
                }

                if (ImGui::SliderAngle("Yaw", &yaw, 0.f)) {
                    light.direction(ComputeDirection(pitch, yaw));
                    m_signalLightChanged(light, i, enabled);
                }

                ImGui::BeginDisabled();
                ImGui::InputFloat3("Direction", glm::value_ptr(light.direction()));
                ImGui::EndDisabled();
                
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}
