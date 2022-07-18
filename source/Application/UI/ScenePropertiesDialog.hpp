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

    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframeEnabled);

protected:
    virtual void defineUI() override;

private:
    sigslot::signal<glm::vec3> m_signalClearColorChanged;

    glm::vec3 m_clearColor{};
    int m_projection = Projection::ePerspective;
    bool m_wireframe = false;
};
