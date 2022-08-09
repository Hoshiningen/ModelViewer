#include "UI/LightPropertiesDialog.hpp"

#include "Camera/Camera.hpp"

#include "Common/Math.hpp"

#include <format>
#include <fstream>

#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

LightPropertiesDialog::LightPropertiesDialog()
    : Dialog() { initialize(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title)
    : Dialog(title) { initialize(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, ImGuiWindowFlags flags)
    : Dialog(title, flags) { initialize(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags)
    : Dialog(title, position, flags) { initialize(); }

LightPropertiesDialog::LightPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags)
    : Dialog(title, position, size, flags) { initialize(); }

std::string_view LightPropertiesDialog::id() const {
    return "LightPropertiesDialog";
}

nlohmann::json LightPropertiesDialog::save() const {

    nlohmann::json json;

    for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {

        nlohmann::json& obj = json[id().data()][std::format("light{}", lightIndex + 1).c_str()];

        obj["yaw"] = m_yaws.at(lightIndex);
        obj["pitch"] = m_pitches.at(lightIndex);
        obj["enabled"] = m_enabledLights.at(lightIndex);
        obj.update(m_lights.at(lightIndex).save());
    }

    return json;
}

void LightPropertiesDialog::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {

        float& pitch = m_pitches.at(lightIndex);
        float& yaw = m_yaws.at(lightIndex);
        bool& enabled = m_enabledLights.at(lightIndex);
        DirectionalLight& light = m_lights.at(lightIndex);

        const std::string lightId = std::format("light{}", lightIndex + 1);
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

            light.direction(RotateVector(kWorldForward, pitch, yaw, false));
            m_sceneLights.at(lightIndex) = enabled ? &light : nullptr;
        }
    }
}

DirectionalLight** LightPropertiesDialog::directionalLight(uint8_t index) {

    if (index < 0 || index >= kMaxLights)
        return nullptr;

    return &m_sceneLights.at(index);
}

void LightPropertiesDialog::defineUI() {

    if (ImGui::BeginTable("", 3)) {

        for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {

            ImGui::TableNextColumn();

            if (lightIndex == 0)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(std::format("Light {}", lightIndex + 1).c_str(), &m_enabledLights.at(lightIndex)))
                m_sceneLights.at(lightIndex) = m_enabledLights.at(lightIndex) ? &m_lights.at(lightIndex) : nullptr;
            
            if (lightIndex == 0)
                ImGui::EndDisabled();
        }

        ImGui::EndTable();
    }
    
    ImGui::Spacing();

    if (ImGui::BeginTabBar("DirectionalLights")) {

        ImGui::Spacing();

        for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {
            
            if (!m_enabledLights.at(lightIndex))
                continue;

            const std::string lightId = std::format("Light {}", lightIndex + 1);
            if (ImGui::BeginTabItem(lightId.c_str())) {

                float& pitch = m_pitches.at(lightIndex);
                float& yaw = m_yaws.at(lightIndex);
                DirectionalLight& light = m_lights.at(lightIndex);

                ImGui::ColorEdit3("Color", glm::value_ptr(light.color()));
                ImGui::SliderFloat("Intensity", &light.intensity(), 0.f, 1.f);

                if (ImGui::SliderAngle("Pitch", &pitch, -90.f, 90.f))
                    light.direction(RotateVector(kWorldForward, pitch, yaw, false));

                if (ImGui::SliderAngle("Yaw", &yaw, 0.f))
                    light.direction(RotateVector(kWorldForward, pitch, yaw, false));

                ImGui::BeginDisabled();
                ImGui::InputFloat3("Direction", glm::value_ptr(light.direction()));
                ImGui::EndDisabled();
                
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}

void LightPropertiesDialog::initialize() {

    m_enabledLights = { true, true, false };
    m_yaws = { glm::radians(0.f), glm::radians(180.f), glm::radians(0.f) };
    m_pitches = { glm::radians(45.f), glm::radians(45.f), glm::radians(-90.f) };

    for (uint8_t lightIndex = 0; lightIndex < kMaxLights; ++lightIndex) {

        DirectionalLight& light = m_lights.at(lightIndex);
        const bool enabled = m_enabledLights.at(lightIndex);
        const float pitch = m_pitches.at(lightIndex);
        const float yaw = m_yaws.at(lightIndex);

        m_sceneLights.at(lightIndex) = enabled ? &light : nullptr;
        light.direction(RotateVector(kWorldForward, pitch, yaw, false));
    }
}
