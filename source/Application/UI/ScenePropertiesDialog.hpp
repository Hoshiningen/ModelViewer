#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <sigslot/signal.hpp>

class ScenePropertiesDialog : public Dialog {
public:
    using Dialog::Dialog;

    enum Projection {
        ePerspective, eOrthographic
    };

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    DEFINE_CONNECTION(m_signalClearColorChanged, ClearColorChanged)
    DEFINE_CONNECTION(m_signalAmbientColorChanged, AmbientColorChanged)

    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframeEnabled);

protected:
    virtual void defineUI() override;

private:
    sigslot::signal<glm::vec3> m_signalClearColorChanged;
    sigslot::signal<glm::vec3, float> m_signalAmbientColorChanged;

    glm::vec3 m_clearColor{};
    glm::vec3 m_ambientColor{ 0.1f, 0.1f, 0.1f };
    float m_ambientIntensity = 1.f;

    int m_projection = Projection::ePerspective;
    bool m_wireframe = false;
};
