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
    using Dialog::Dialog;

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalLightChanged, LightChanged)

    void onModelLoaded(std::forward_list<VertexBuffered>* pModel);

protected:
    virtual void defineUI() override;

private:
    // Signals
    sigslot::signal<const DirectionalLight&, uint8_t, bool> m_signalLightChanged;

    std::array<bool, kMaxLights> m_enabledLights{ true, false, false };

    std::array<float, kMaxLights> m_yaws{
        glm::radians(0.f), glm::radians(120.f), glm::radians(240.f)
    };

    std::array<float, kMaxLights> m_pitches{
        glm::radians(-60.f), glm::radians(-60.f), glm::radians(-60.f)
    };

    std::array<DirectionalLight, kMaxLights> m_lights;
};