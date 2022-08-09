#pragma once

#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"
#include "Common/Constants.hpp"

#include "Light/DirectionalLight.hpp"

#include <array>
#include <forward_list>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <sigslot/signal.hpp>

class VertexBuffered;

class LightPropertiesDialog : public Dialog {
public:
    LightPropertiesDialog();
    LightPropertiesDialog(const std::string& title);
    LightPropertiesDialog(const std::string& title, ImGuiWindowFlags flags);
    LightPropertiesDialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags);
    LightPropertiesDialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags);

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DirectionalLight** directionalLight(uint8_t index);

protected:
    virtual void defineUI() override;

private:
    void initialize();

    std::array<bool, kMaxLights> m_enabledLights;
    std::array<float, kMaxLights> m_yaws;
    std::array<float, kMaxLights> m_pitches;

    std::array<DirectionalLight, kMaxLights> m_lights;
    std::array<DirectionalLight*, kMaxLights> m_sceneLights;
};